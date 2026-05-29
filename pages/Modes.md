# Modes

`oj_windows` provides multiple modes to control JSON serialization and parsing behavior.

## Available Modes

| Mode | Description |
|------|-------------|
| `:strict` | Strict JSON compliance, native types only |
| `:null` | Like strict, but outputs null for unsupported types |
| `:compat` / `:json` | JSON gem compatible |
| `:rails` | ActiveSupport/Rails compatible |
| `:object` | Full Ruby object serialization |
| `:custom` | Maximum flexibility, honors all options |
| `:wab` | WAB format for web applications |

## Mode Details

### :strict Mode

Strict mode follows JSON specifications exactly. Only native JSON types are supported:
- Boolean (`true`, `false`)
- `nil`
- String
- Hash
- Array
- Numbers

Non-native types raise an Exception. This is the safest mode for parsing untrusted input.

### :null Mode

Similar to `:strict`, but outputs JSON `null` for non-native types instead of raising an Exception.

### :compat Mode

Mimics the Ruby JSON gem. Uses `to_json(*)` methods for serialization.
See [JsonGem.md](JsonGem.md) for details.

```ruby
Oj.mimic_JSON()  # Take over JSON module methods
```

### :rails Mode

Mimics ActiveSupport's encoder. Uses `as_json(*)` methods for serialization.
See [Rails.md](Rails.md) for details.

```ruby
Oj.optimize_rails()  # Full Rails integration
```

### :object Mode

Full Ruby object serialization and deserialization. Encodes class information,
circular references, and object IDs in the JSON output.
See [Encoding.md](Encoding.md) for the encoding format.

### :custom Mode

Maximum flexibility with all options honored. Cannot exactly replicate other modes
but provides the most control. See [Custom.md](Custom.md) for details.

### :wab Mode

Optimized for WAB (Web Application Builder) data exchange. Only honors the indent option.
See [WAB.md](WAB.md) for details.

## Setting the Mode

```ruby
require 'oj_windows'

# Set default mode
Oj.default_options = { mode: :compat }

# Or per-call
json = Oj.dump(data, mode: :strict)
data = Oj.load(json, mode: :strict)
```

## Mode Consistency

Use the same mode for both dumping and loading. Mixing modes may produce unexpected results.

```ruby
# Correct: same mode for dump and load
json = Oj.dump(obj, mode: :object)
obj2 = Oj.load(json, mode: :object)

# Incorrect: mixed modes
json = Oj.dump(obj, mode: :object)
obj2 = Oj.load(json, mode: :strict)  # May fail or lose data
```
