# Changelog

All notable changes to `oj_windows` will be documented in this file.

## [3.17.2.1] - 2026-05-29

Fork revision based on upstream Oj 3.17.2. The version is `3.17.2.1` (not plain
`3.17.2`) because this carries upstream's parser feature set through 3.17.2 plus
the MSVC/SIMD adaptations below; it is not a byte-identical rebase of 3.17.2.

### Performance
- Enabled SIMD string scanning under MSVC. `simd.h` previously gated SSE on the
  GCC/Clang `__SSE2__`/`__SSE4_2__` feature macros, which `cl.exe` never defines,
  so the scalar scanner was always used on the only supported platform. SSE2 is
  now enabled by default on x86_64 (~20–25% faster parsing on string-heavy
  payloads). Added MSVC shims for the GCC builtins used by the SSE scanners
  (`__builtin_prefetch`/`__builtin_expect`/`__builtin_ctz`).
- The SSE4.2 (PCMPESTRI) scanner is also compiled and selectable, but SSE2 is the
  default because PCMPESTRI is slower on common microarchitectures.
- Added a `--disable-simd` build option and an `OJ_SCAN=scalar|sse2|sse42` runtime
  override (replacing the previously non-functional `--with-sse42` flag).

### Security / robustness (ported from upstream Oj 3.16.16–3.17.2)
- `Oj::Parser` (the "usual" parser) now validates that the document is complete:
  incomplete literals (`tru`, `nul`, `n`, …) and unclosed arrays/objects (`[1,2`,
  `{"a":1`) are rejected instead of being silently accepted. Ported upstream's
  `validate_document_end` (raises `EncodingError`/`Oj::ParserError` with
  "expected …" / "… is not closed"). `test_parser_usual.rb#test_nil` was updated
  to match the stricter, upstream behavior.
- Added the **`Oj::Parser.safe`** parser (upstream 3.17.0): builds Ruby objects
  like `:usual` but enforces configurable limits on untrusted input —
  `:max_hash_size`, `:max_array_size`, `:max_depth`, `:max_total_elements` —
  raising `Oj::Parser::ValidationError` subclasses (`HashSizeError`,
  `ArraySizeError`, `DepthError`, `TotalElementsError`). Ported `safe.c`/`safe.h`.
- Added the upstream 3.17.2 "extreme sizes" key-length guard: object keys longer
  than 32,000 bytes now raise instead of overflowing the `int16_t` key length.
- Fixed an uninitialized-pointer dereference in the streaming parser: `sparse.c`
  never set `ni.pi`, but `oj_num_as_value` dereferences `ni->pi->err_class` on the
  invalid-float path. Initialized `ni.pi` at all three number sites (matches
  upstream).
- Note: the non-Windows file-read partial-read fix (#1004) is not reachable on
  MSVC (that path is `#if !IS_WINDOWS`); the Windows-reachable `Oj::Parser#file`
  read was fixed separately (see below).

### Fixed
- `mem.c` (the `MEM_DEBUG` allocator) no longer fails to compile on MSVC: it now
  uses a Windows `SRWLOCK` (which has a static initializer) instead of an
  unguarded `pthread_mutex_t`.
- `Oj::Parser#file` captured the `read()` result in an unsigned `size_t`; on a
  read error (`-1`) this wrapped to `SIZE_MAX`, causing an out-of-bounds write
  and making the error branch unreachable. It now uses a signed count, handles
  errors/EOF correctly, and closes the file descriptor.
- Guarded the remaining unconditional `<sys/types.h>` includes (`reader.c`,
  `rxclass.c`) and added a guarded `ssize_t` fallback for MSVC.

### Changed
- Removed the dead, byte-identical `lib/oj/` duplicate tree from the gem package;
  `lib/oj_windows/` is the only tree that is ever loaded.
- gemspec now credits Peter Ohler and states the relationship to upstream Oj
  (oj_windows replaces, and cannot be co-installed with, the `oj` gem).
- Build scripts (`package_install.bat`, `compile.bat`, `build_install.bat`) locate
  Visual Studio via `vswhere` (and put the Installer dir on PATH so vcvars runs
  cleanly) and derive the gem filename from `Oj::VERSION` instead of hardcoding a
  VS path or version number.
- Added a GitHub Actions CI workflow (`.github/workflows/ci.yml`) using the mswin
  (MSVC) Ruby: it compiles the extension, runs the core test suite, and verifies a
  clean `gem install` against the Ruby headers alone.
- `extconf.rb` no longer runs `nmake clean`/`make clean` after `create_makefile`
  (it ran during `gem install`, just before the build, so it was pointless noise).
- README: honest performance framing, a CI badge in place of the static
  "373 Passing"/"High Performance" badges, and corrected test/skip counts (5 skips).

## [3.16.15] - 2025-12-20

- Published release following 3.16.14. This entry was added retroactively to
  document the version that was actually shipped to RubyGems.

## [3.16.14] - 2025-12-20

### Initial Release

First production-ready release of `oj_windows`, a Windows-exclusive fork optimized for Ruby 3.4.8 MSVC.

#### C Extension Modernization
- Replaced `pthread` mutexes with Windows `CRITICAL_SECTION`
- Replaced `timegm` with `_mkgmtime` for MSVC compatibility
- Guarded all POSIX headers (`unistd.h`, `sys/types.h`, `poll.h`)
- Resolved `NAN`/`HUGE_VAL` math macro incompatibilities for MSVC
- Removed all GCC/Clang specific compiler flags

#### Build System
- Updated `extconf.rb` for MSVC/NMake build process
- Updated `Rakefile` to remove Unix-specific logic
- Created `package_install.bat` for Windows build workflow

#### Gem Structure
- Refactored internal paths from `oj` to `oj_windows`
- Created `lib/oj/` compatibility layer for legacy `rb_require` calls
- Updated gemspec for Windows-only distribution

#### Testing
- Converted shell test scripts to Windows batch files
- Fixed test regex patterns for Windows path formats
- Verified 100% pass rate (373 runs, 0 failures)

#### Documentation
- Cleaned up all documentation for Windows-only usage
- Removed cross-platform references
- Added test results table to README

### Test Results Summary

```
Total: 373 runs, 8,860 assertions, 0 failures, 0 errors, 6 skips
```

The 6 skipped tests are fork-based operations not supported on Windows.
