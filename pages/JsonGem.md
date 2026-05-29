# JSON Gem Compatibility

`oj_windows` can serve as a drop-in replacement for the Ruby JSON gem with improved performance on Windows MSVC.

## Quick Start

To have `oj_windows` take over the `JSON` constant methods (`load`, `parse`, etc.):

```ruby
require 'oj_windows'

Oj.mimic_JSON()
```

If your project already requires the `json` gem, call `Oj.mimic_JSON()` after the json gem has been loaded.

## Optimized Type Encoding

For faster encoding of common types, add them to Oj's optimized list:

```ruby
Oj.add_to_json(Array, BigDecimal, Complex, Date, DateTime, Exception, Hash, Integer, OpenStruct, Range, Rational, Regexp, Struct, Time)

# Or add all available types:
# Oj.add_to_json()
```

To revert a type to unoptimized encoding:

```ruby
Oj.remove_to_json(Rational)
```

## Supported Types

The following types can be added for optimized encoding:

| Type | Description |
|------|-------------|
| Array | Ruby arrays |
| BigDecimal | Arbitrary precision decimals |
| Complex | Complex numbers |
| Date | Date objects |
| DateTime | Date with time |
| Exception | Exception objects |
| Hash | Ruby hashes |
| Integer | Integer values |
| OpenStruct | OpenStruct objects |
| Range | Range objects |
| Rational | Rational numbers |
| Regexp | Regular expressions |
| Struct | Struct objects |
| Time | Time objects |

## Compatibility Notes

- `oj_windows` in `:compat` mode mimics the json gem's `to_json(*)` method behavior
- JSON.generate(), JSON.dump(), and JSON() functions are supported
- NaN and Infinity handling matches the json gem's behavior
- Compatibility target is json gem version 2.0.3
