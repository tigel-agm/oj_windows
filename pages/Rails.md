# Rails Integration

`oj_windows` provides seamless integration with Rails and ActiveSupport on Windows MSVC Ruby.

## Quick Start

Add this to a Rails initializer to replace Rails' JSON handling with `oj_windows`:

```ruby
require 'oj_windows'

Oj.optimize_rails()
```

This single call:
- Sets `oj_windows` as the JSON encoder
- Sets `oj_windows` as the JSON decoder
- Optimizes common Rails types
- Mimics the JSON gem interface

## Manual Setup

For more control, configure each component separately:

```ruby
require 'oj_windows'

Oj::Rails.set_encoder()   # Use Oj for encoding
Oj::Rails.set_decoder()   # Use Oj for decoding
Oj::Rails.optimize()      # Optimize common types
Oj::Rails.mimic_JSON()    # Mimic JSON gem interface
```

## Optimized Types

When `Oj::Rails.optimize()` is called, these types use fast native encoding:

| Type | Description |
|------|-------------|
| Array | Ruby arrays |
| BigDecimal | Arbitrary precision decimals |
| Float | Floating point numbers |
| Hash | Ruby hashes |
| Range | Range objects |
| Regexp | Regular expressions |
| Time | Time objects |
| ActiveSupport::TimeWithZone | Rails time zones |
| ActionController::Parameters | Controller params |
| ActiveRecord::Base subclasses | Model instances |

## ActiveSupport Globals

`oj_windows` honors these ActiveSupport encoding globals:

```ruby
ActiveSupport::JSON::Encoding.use_standard_json_time_format
ActiveSupport::JSON::Encoding.escape_html_entities_in_json
ActiveSupport::JSON::Encoding.time_precision
ActiveSupport::JSON::Encoding.json_encoder
```

## Verifying Oj is Active

To confirm `oj_windows` is handling JSON encoding, enable tracing:

```ruby
require 'oj_windows'

Oj.default_options = { trace: true }
Oj.optimize_rails()

Time.now.to_json  # Will output trace information
```

## Notes

1. **Float Precision**: Optimized floats use 16 significant digits (Ruby's json gem may use 16 or 17 depending on value)

2. **Hash Key Collisions**: Optimized hashes don't check for duplicate keys after `to_s()` conversion

3. **Rails Mode**: Calling `Oj.optimize_rails()` doesn't change the default Oj mode. To also use Rails mode for direct Oj calls:

```ruby
Oj.default_options = { mode: :rails }
```
