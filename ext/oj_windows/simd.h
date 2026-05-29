#ifndef OJ_SIMD_H
#define OJ_SIMD_H

// SIMD architecture detection and configuration
// This header provides unified SIMD support across different CPU architectures

// Escape hatch: build with -DOJ_DISABLE_SIMD to force the scalar scanner on any
// platform (useful for debugging or comparison benchmarks).
#ifndef OJ_DISABLE_SIMD

// x86/x86_64 SIMD detection
#if defined(__x86_64__) || defined(__i386__) || defined(_M_IX86) || defined(_M_X64)
#define HAVE_SIMD_X86 1

#if defined(_MSC_VER)
// MSVC (cl.exe) does not define the GCC/Clang feature macros __SSE2__/__SSE4_2__,
// and it requires no -msse flag to emit SSE intrinsics. On x64, SSE2 is guaranteed
// by the architecture, so we enable the SSE2 scanner unconditionally. SSE4.2 is not
// guaranteed by every CPU, so we also compile the SSE4.2 scanner but select it only
// at runtime via CPUID (see oj_scanner_init() in parse.c).
#if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define HAVE_SIMD_SSE2 1
#include <emmintrin.h>
#define HAVE_SIMD_SSE4_2 1
#define OJ_SIMD_SSE4_2_RUNTIME 1
#include <nmmintrin.h>
#include <intrin.h>  // __cpuid, _BitScanForward
#endif

#else  // GCC / Clang: gate on the -msseN feature macros set by extconf.rb.

// SSE4.2 support (Intel Core i7+, AMD Bulldozer+)
// Enabled automatically when compiler has -msse4.2 flag
#if defined(__SSE4_2__)
#define HAVE_SIMD_SSE4_2 1
#include <nmmintrin.h>
#endif

// SSE2 support (fallback for older x86_64 CPUs - all x86_64 CPUs support SSE2)
#if defined(__SSE2__) && !defined(HAVE_SIMD_SSE4_2)
#define HAVE_SIMD_SSE2 1
#include <emmintrin.h>
#endif

#endif  // _MSC_VER

#endif  // x86/x86_64

// ARM NEON detection
#if defined(__ARM_NEON) || defined(__ARM_NEON__) || defined(__aarch64__) || defined(_M_ARM64)
#define HAVE_SIMD_NEON 1
#define SIMD_MINIMUM_THRESHOLD 6
#include <arm_neon.h>
#endif

#endif  // OJ_DISABLE_SIMD

// Define which SIMD implementation to use (priority order: SSE4.2 > NEON > SSE2)
#if defined(HAVE_SIMD_SSE4_2)
#define HAVE_SIMD_STRING_SCAN 1
#define SIMD_TYPE "SSE4.2"
#elif defined(HAVE_SIMD_NEON)
#define HAVE_SIMD_STRING_SCAN 1
#define SIMD_TYPE "NEON"
#elif defined(HAVE_SIMD_SSE2)
#define HAVE_SIMD_STRING_SCAN 1
#define SIMD_TYPE "SSE2"
#else
#define SIMD_TYPE "none"
#endif

// Portability shims: the SSE scanners in parse.c are written with GCC/Clang
// builtins. Provide MSVC-native equivalents so the same code compiles under
// cl.exe. Only defined when an x86 SSE path is actually enabled, and guarded by
// #ifndef so a real GCC/Clang build is never affected.
#if defined(_MSC_VER) && (defined(HAVE_SIMD_SSE2) || defined(HAVE_SIMD_SSE4_2))
#ifndef __builtin_prefetch
#define __builtin_prefetch(addr, ...) _mm_prefetch((const char *)(addr), _MM_HINT_T0)
#endif
#ifndef __builtin_expect
#define __builtin_expect(expr, expected) (expr)
#endif
static __forceinline int oj_msvc_ctz(unsigned int x) {
    unsigned long index;
    _BitScanForward(&index, x);  // callers guarantee x != 0
    return (int)index;
}
#ifndef __builtin_ctz
#define __builtin_ctz(x) oj_msvc_ctz((unsigned int)(x))
#endif
#endif

#endif /* OJ_SIMD_H */
