// Copyright (c) 2018, Peter Ohler, All rights reserved.

#include "mem.h"

#if !IS_WINDOWS
#include <pthread.h>
#endif
#include <ruby.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _rec {
    struct _rec *next;
    const void  *ptr;
    size_t       size;
    const char  *file;
    int          line;
    bool         ruby;
} *Rec;

typedef struct _rep {
    struct _rep *next;
    size_t       size;
    const char  *file;
    int          line;
    int          cnt;
} *Rep;

#ifdef MEM_DEBUG

#if IS_WINDOWS
// Included here (after ruby.h, which already pulls in winsock2/windows on mswin)
// to avoid the winsock.h vs winsock2.h redefinition clash that occurs if
// <windows.h> is included ahead of Ruby's headers.
#include <windows.h>
// CRITICAL_SECTION has no static initializer, but SRWLOCK does (SRWLOCK_INIT)
// and is the natural Windows equivalent for this simple debug-only lock.
static SRWLOCK lock = SRWLOCK_INIT;
#define MEM_LOCK()   AcquireSRWLockExclusive(&lock)
#define MEM_UNLOCK() ReleaseSRWLockExclusive(&lock)
#else
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#define MEM_LOCK()   pthread_mutex_lock(&lock)
#define MEM_UNLOCK() pthread_mutex_unlock(&lock)
#endif
static Rec             recs      = NULL;
static const char      mem_pad[] = "--- This is a memory pad and should not change until being freed. ---";

void *oj_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size + sizeof(mem_pad));

    if (NULL != ptr) {
        Rec r = (Rec)malloc(sizeof(struct _rec));

        if (NULL != r) {
            strcpy(((char *)ptr) + size, mem_pad);
            r->ptr  = ptr;
            r->size = size;
            r->file = file;
            r->line = line;
            r->ruby = false;
            MEM_LOCK();
            r->next = recs;
            recs    = r;
            MEM_UNLOCK();
        } else {
            free(ptr);
            ptr = NULL;
        }
    }
    return ptr;
}

void *oj_realloc(void *orig, size_t size, const char *file, int line) {
    void *ptr = realloc(orig, size + sizeof(mem_pad));
    Rec   r;

    if (NULL != ptr) {
        strcpy(((char *)ptr) + size, mem_pad);
        MEM_LOCK();
        for (r = recs; NULL != r; r = r->next) {
            if (orig == r->ptr) {
                r->ptr  = ptr;
                r->size = size;
                r->file = file;
                r->line = line;
                r->ruby = false;
                break;
            }
        }
        MEM_UNLOCK();
        if (NULL == r) {
            printf("Realloc at %s:%d (%p) not allocated.\n", file, line, orig);
        }
    }
    return ptr;
}

void *oj_calloc(size_t count, size_t size, const char *file, int line) {
    void *ptr;

    size *= count;
    if (NULL != (ptr = malloc(size + sizeof(mem_pad)))) {
        Rec r = (Rec)malloc(sizeof(struct _rec));

        if (NULL != r) {
            memset(ptr, 0, size);
            strcpy(((char *)ptr) + size, mem_pad);
            r->ptr  = ptr;
            r->size = size;
            r->file = file;
            r->line = line;
            r->ruby = false;
            MEM_LOCK();
            r->next = recs;
            recs    = r;
            MEM_UNLOCK();
        } else {
            free(ptr);
            ptr = NULL;
        }
    }
    return ptr;
}

void *oj_r_alloc(size_t size, const char *file, int line) {
    void *ptr = ruby_xmalloc(size + sizeof(mem_pad));

    if (NULL != ptr) {
        Rec r = (Rec)malloc(sizeof(struct _rec));

        if (NULL != r) {
            strcpy(((char *)ptr) + size, mem_pad);
            r->ptr  = ptr;
            r->size = size;
            r->file = file;
            r->line = line;
            r->ruby = true;
            MEM_LOCK();
            r->next = recs;
            recs    = r;
            MEM_UNLOCK();
        } else {
            free(ptr);
            ptr = NULL;
        }
    }
    return ptr;
}

void *oj_r_realloc(void *orig, size_t size, const char *file, int line) {
    void *ptr = ruby_xrealloc2(orig, 1, size + sizeof(mem_pad));
    Rec   r;

    if (NULL != ptr) {
        strcpy(((char *)ptr) + size, mem_pad);
        MEM_LOCK();
        for (r = recs; NULL != r; r = r->next) {
            if (orig == r->ptr) {
                r->ptr  = ptr;
                r->size = size;
                r->file = file;
                r->line = line;
                r->ruby = true;
                break;
            }
        }
        MEM_UNLOCK();
        if (NULL == r) {
            printf("Realloc at %s:%d (%p) not allocated.\n", file, line, orig);
        }
    }
    return ptr;
}

void oj_freed(void *ptr, const char *file, int line, bool ruby) {
    if (NULL != ptr) {
        Rec r    = NULL;
        Rec prev = NULL;

        MEM_LOCK();
        for (r = recs; NULL != r; r = r->next) {
            if (ptr == r->ptr) {
                if (NULL == prev) {
                    recs = r->next;
                } else {
                    prev->next = r->next;
                }
                break;
            }
            prev = r;
        }
        MEM_UNLOCK();
        if (NULL == r) {
            printf("Free at %s:%d (%p) not allocated or already freed.\n", file, line, ptr);
        } else {
            char *pad = (char *)r->ptr + r->size;

            if (r->ruby != ruby) {
                if (r->ruby) {
                    printf("Memory at %s:%d (%p) allocated with Ruby allocator and freed with stdlib free.\n",
                           file,
                           line,
                           ptr);
                } else {
                    printf("Memory at %s:%d (%p) allocated with stdlib allocator and freed with Ruby free.\n",
                           file,
                           line,
                           ptr);
                }
            }
            if (0 != strcmp(mem_pad, pad)) {
                uint8_t *p;
                uint8_t *end = (uint8_t *)pad + sizeof(mem_pad);

                printf("Memory at %s:%d (%p) write outside allocated.\n", file, line, ptr);
                for (p = (uint8_t *)pad; p < end; p++) {
                    if (0x20 < *p && *p < 0x7f) {
                        printf("%c  ", *p);
                    } else {
                        printf("%02x ", *(uint8_t *)p);
                    }
                }
                printf("\n");
            }
            free(r);
        }
    }
}

void oj_r_free(void *ptr, const char *file, int line) {
    oj_freed(ptr, file, line, true);
    xfree(ptr);
}

void oj_free(void *ptr, const char *file, int line) {
    oj_freed(ptr, file, line, false);
    free(ptr);
}

char *oj_mem_strdup(const char *str, const char *file, int line) {
    size_t size = strlen(str) + 1;
    char  *ptr  = (char *)malloc(size + sizeof(mem_pad));

    if (NULL != ptr) {
        Rec r = (Rec)malloc(sizeof(struct _rec));

        if (NULL != r) {
            strcpy(ptr, str);
            strcpy(((char *)ptr) + size, mem_pad);
            r->ptr  = (void *)ptr;
            r->size = size;
            r->file = file;
            r->line = line;
            r->ruby = false;
            MEM_LOCK();
            r->next = recs;
            recs    = r;
            MEM_UNLOCK();
        } else {
            free(ptr);
            ptr = NULL;
        }
    }
    return ptr;
}

#endif

#ifdef MEM_DEBUG

static Rep update_reps(Rep reps, Rec r) {
    Rep rp = reps;

    for (; NULL != rp; rp = rp->next) {
        if (rp->line == r->line && (rp->file == r->file || 0 == strcmp(rp->file, r->file))) {
            rp->size += r->size;
            rp->cnt++;
            break;
        }
    }
    if (NULL == rp && NULL != (rp = (Rep)malloc(sizeof(struct _rep)))) {
        rp->size = r->size;
        rp->file = r->file;
        rp->line = r->line;
        rp->cnt  = 1;
        rp->next = reps;
        reps     = rp;
    }
    return reps;
}

static void print_stats() {
    printf("\n--- Memory Usage Report --------------------------------------------------------\n");
    MEM_LOCK();

    if (NULL == recs) {
        printf("No memory leaks\n");
    } else {
        Rep    reps = NULL;
        Rep    rp;
        Rec    r;
        size_t leaked = 0;

        for (r = recs; NULL != r; r = r->next) {
            reps = update_reps(reps, r);
        }
        while (NULL != (rp = reps)) {
            reps = rp->next;
            printf("%16s:%3d %8lu bytes over %d occurances allocated and not freed.\n",
                   rp->file,
                   rp->line,
                   rp->size,
                   rp->cnt);
            leaked += rp->size;
            free(rp);
        }
        printf("%lu bytes leaked\n", leaked);
    }
    MEM_UNLOCK();
    printf("--------------------------------------------------------------------------------\n");
}

#endif

void oj_mem_report(void) {
#ifdef MEM_DEBUG
    rb_gc();
    print_stats();
#endif
}
