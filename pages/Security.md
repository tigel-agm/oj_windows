# Security Considerations

When parsing JSON from untrusted sources, certain `oj_windows` options should be used with caution.

## Potential Vulnerabilities

### Symbol Keys
The `:symbol_keys` option converts JSON keys to Ruby Symbols. In Ruby versions before 2.2, Symbols are not garbage collected, which could lead to memory exhaustion from malicious input.

### Auto-Define Classes
The `:auto_define` option automatically creates Ruby classes from JSON. While useful during development, this can exhaust memory if an attacker sends JSON with many unique class names.

## Safe Parsing

For untrusted input, use `Oj.safe_load()` which applies the strictest, safest options:

```ruby
require 'oj_windows'

# Safe parsing of untrusted JSON
data = Oj.safe_load(untrusted_json_string)
```

## Best Practices

1. **Use `safe_load()` for untrusted input** - applies safest defaults
2. **Avoid `:symbol_keys` with untrusted data** - prevents Symbol table exhaustion
3. **Disable `:auto_define` in production** - prevents arbitrary class creation
4. **Validate parsed data** - always validate structure and types after parsing
5. **Set `:max_nesting`** - limit depth to prevent stack overflow attacks

## Object Mode Considerations

In `:object` mode, `oj_windows` can deserialize Ruby objects from JSON. While no methods are called during deserialization, you should always validate the resulting objects before using them.

```ruby
# Safe: strict mode only creates native JSON types
data = Oj.strict_load(json_string)

# Caution: object mode creates Ruby objects
obj = Oj.object_load(json_string)
# Validate obj before using
```
