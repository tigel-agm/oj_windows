# oj_windows

**A Windows-native, high-performance JSON parser and Object marshaller for Ruby.**

[![Gem Version](https://img.shields.io/gem/v/oj_windows.svg?style=flat-square)](https://rubygems.org/gems/oj_windows)
[![Ruby](https://img.shields.io/badge/Ruby-3.4%2B-CC342D?style=flat-square&logo=ruby&logoColor=white)](https://www.ruby-lang.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20MSVC-0078D6?style=flat-square&logo=windows&logoColor=white)](https://github.com/tigel-agm/oj_windows)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)](https://github.com/tigel-agm/oj_windows/blob/main/LICENSE)
[![CI](https://github.com/tigel-agm/oj_windows/actions/workflows/ci.yml/badge.svg)](https://github.com/tigel-agm/oj_windows/actions/workflows/ci.yml)

## Overview

`oj_windows` is a fork of [Oj](https://github.com/ohler55/oj) by Peter Ohler, adapted to build with the **MSVC (mswin)** Ruby toolchain. POSIX-specific code has been guarded or replaced with native Windows equivalents (pthread mutexes → Windows synchronization, `timegm` → `_mkgmtime`, guarded POSIX headers), and SIMD string scanning is enabled under MSVC.

> **Relationship to upstream Oj.** Almost all of this code is Peter Ohler's. `oj_windows` exists only to provide a buildable Oj for Ruby compiled with the MSVC toolchain. It defines the same `Oj` module and the same public API, so **it is a drop-in replacement for — and cannot be installed alongside — the upstream `oj` gem**. If you use the standard **RubyInstaller / MinGW** Ruby, install the upstream [`oj`](https://rubygems.org/gems/oj) gem instead (it builds there via the MSYS2 devkit); `oj_windows` is only needed for native MSVC (`mswin`) builds.

## Requirements

| Component | Version |
|-----------|---------|
| Ruby | 3.4.8+ (MSVC build) |
| OS | Windows 10/11 x64 |
| Compiler | Visual Studio 2022+ |
| Architecture | x64-mswin64_140 |

## Installation

```powershell
gem install oj_windows
```

Or in your Gemfile:

```ruby
gem 'oj_windows'
```

## Quick Start

```ruby
require 'oj_windows'

# Dump Ruby objects to JSON
hash = { 'name' => 'oj_windows', 'version' => '3.17.2.1', 'windows' => true }
json = Oj.dump(hash)
# => {"name":"oj_windows","version":"3.16.14","windows":true}

# Load JSON back to Ruby objects  
data = Oj.load(json)
# => {"name"=>"oj_windows", "version"=>"3.16.14", "windows"=>true}

# Pretty print
puts Oj.dump(hash, indent: 2)
```

## Features

- **SIMD-accelerated** C extension compiled with MSVC (SSE2 string scanning on x86_64)
- **Multiple Modes**: Strict, Compat, Object, Custom, Rails, WAB, Null
- **JSON Gem Compatibility**: Drop-in replacement via `Oj.mimic_JSON`
- **Rails Integration**: `:rails` mode and `Oj.optimize_rails` for ActiveSupport/ActiveRecord (see note under Test Results)
- **Streaming**: SAJ and SC parsers for large documents
- **Safe parsing**: `Oj::Parser.safe(max_depth:, max_array_size:, max_hash_size:, max_total_elements:)` enforces limits on untrusted input
- **StringWriter/StreamWriter**: Efficient JSON generation

## Test Results

All tests pass on Windows MSVC Ruby 3.4.8:

| Test Suite | Runs | Assertions | Failures | Errors | Skips |
|------------|------|------------|----------|--------|-------|
| test_various.rb | 69 | 8,202 | 0 | 0 | 1 |
| test_strict.rb | 42 | 71 | 0 | 0 | 0 |
| test_compat.rb | 55 | 112 | 0 | 0 | 0 |
| test_object.rb | 74 | 161 | 0 | 0 | 0 |
| test_fast.rb | 43 | 137 | 0 | 0 | 0 |
| test_saj.rb | 13 | 18 | 0 | 0 | 0 |
| test_scp.rb | 23 | 23 | 0 | 0 | 2 |
| test_gc.rb | 3 | 50 | 0 | 0 | 0 |
| test_writer.rb | 27 | 28 | 0 | 0 | 1 |
| test_file.rb | 21 | 52 | 0 | 0 | 1 |
| test_hash.rb | 3 | 7 | 0 | 0 | 0 |
| **Total** | **373** | **8,861** | **0** | **0** | **5** |

> **Scope**: This is the core suite run by `test/test_all_no_rails.bat`. Additional mode/parser tests (`test_custom`, `test_null`, `test_wab`, `test_generate`, `test_parser_usual`, `test_integer_range`, `test_long_strings`) also pass. A few files depend on POSIX fixtures or a specific working directory (`test_realworld` needs `test/data/*.json`; `test_parser_saj`'s file test needs a local fixture) and are not part of this table. The Rails ActiveSupport/ActiveRecord suites require `fork` and are not run on Windows.
>
> **Skips (5)**: 3 require `fork` (`test_various` ×1, `test_scp` ×2), 1 needs a POSIX subprocess (`test_writer`), and 1 involves pre-1970 dates the Windows time API does not support (`test_file`).

## Performance

`oj_windows` enables SIMD-accelerated string scanning on x86_64 (SSE2 by default —
see [InstallOptions](pages/InstallOptions.md)). On string-heavy payloads this makes
parsing roughly **20–25% faster** than the scalar build that earlier versions
shipped on MSVC.

A note on honest benchmarking: Ruby's bundled `JSON::Ext` is extremely well
optimized and is competitive with — and on small, simple payloads can exceed —
`oj_windows` on raw parse/dump throughput. Oj's value is less about winning a
micro-benchmark and more about capabilities the JSON gem does not offer:

- **Object mode** — full Ruby object serialization/deserialization
- **Rails / ActiveSupport** integration (`Oj.optimize_rails`)
- **Streaming** parsers (SAJ, SCP) for large documents
- **Custom** serialization behavior and circular-reference handling

Benchmark your own representative payloads with the scripts in `test/perf_*.rb`
rather than relying on synthetic numbers.

## Modes

| Mode | Description |
|------|-------------|
| `:strict` | Strict JSON compliance, only native types |
| `:compat` | Compatible with the JSON gem |
| `:object` | Full Ruby object serialization |
| `:custom` | Customizable serialization behavior |
| `:rails` | Rails and ActiveSupport compatibility |
| `:wab` | WAB (Web Application Builder) format |
| `:null` | Return null for unsupported types |

## Configuration

```ruby
# Set default options
Oj.default_options = {
  mode: :compat,
  symbol_keys: true,
  indent: 2
}

# Check current options
Oj.default_options
```

## Documentation

- [Options](pages/Options.md) - Parse and dump options
- [Modes](pages/Modes.md) - Detailed mode documentation
- [Rails](pages/Rails.md) - Rails integration guide
- [JsonGem](pages/JsonGem.md) - JSON gem compatibility
- [Advanced](pages/Advanced.md) - Advanced features

## Building from Source

```powershell
# Requires a Visual Studio (2019+) MSVC build environment. package_install.bat
# locates vcvars64.bat automatically via vswhere, so just run:
.\package_install.bat
```

> If you prefer to set up the environment yourself, run the `vcvars64.bat` from
> your Visual Studio install (e.g. `...\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat`)
> first, then `bundle exec rake compile`.

## Running Tests

```powershell
cd test
.\test_all_no_rails.bat
```

## License

MIT License - See [LICENSE](LICENSE) for details.

---

*Version 3.17.2.1 — based on [Oj](https://github.com/ohler55/oj) 3.17.2 by Peter Ohler (MIT).*
