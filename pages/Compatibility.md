# Compatibility

`oj_windows` is a **Windows-exclusive** fork optimized for the MSVC toolchain.

## Platform Support

| Platform | Status |
|----------|--------|
| Windows (MSVC) | ✅ Fully supported |
| Windows (MinGW) | ❌ Not supported |
| Linux | ❌ Not supported |
| macOS | ❌ Not supported |

## Ruby Version

| Version | Status |
|---------|--------|
| Ruby 3.4.8+ (MSVC) | ✅ Fully supported |
| Older Ruby versions | ❌ Not supported |

## Requirements

- **Ruby**: 3.4.8+ compiled with MSVC
- **OS**: Windows 10/11 x64
- **Compiler**: Visual Studio 2022+
- **Architecture**: x64-mswin64_140

## Rails Compatibility

`oj_windows` is compatible with:

| Rails Version | Status |
|---------------|--------|
| Rails 7.x | ✅ Supported |
| Rails 8.x | ✅ Supported |

Note: Rails projects may use [multi_json](https://github.com/intridea/multi_json) which will automatically prefer oj_windows if installed.

## JSON Gem Compatibility

`oj_windows` can serve as a drop-in replacement for the Ruby JSON gem:

```ruby
require 'oj_windows'

Oj.mimic_JSON()
```

See [JsonGem.md](JsonGem.md) for full compatibility details.
