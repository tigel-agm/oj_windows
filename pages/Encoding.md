# Object Mode Encoding

Object mode in `oj_windows` provides fast Ruby object serialization and deserialization.
In `:object` mode, JSON is generated following conventions that allow Class information
and Object IDs for circular reference detection to be encoded in the JSON document.

## Encoding Rules

### Native Types
JSON native types (`true`, `false`, `nil`, `String`, `Hash`, `Array`, `Number`) are
encoded normally.

### Symbols
A Symbol is encoded as a JSON string with a preceding `':'` character.

### Special Key Markers
The `'^'` character denotes a special key value in a JSON Object sequence.

| Key | Description |
|-----|-------------|
| `"^c"` | Ruby class. Example: `{"^c":"MyModule::MyClass"}` |
| `"^t"` | Ruby Time. Example: `{"^t":1325775487.000000}` |
| `"^o"` | Ruby Object with instance variables |
| `"^O"` | Built-in/odd classes (Rational, Date, DateTime) |
| `"^u"` | Ruby Struct. Example: `{"^u":["Range",1,7,false]}` |
| `"^i"` | Object ID for circular reference detection |
| `"^r"` | Reference to previously encoded Object |

### Object Encoding Example

```json
{"^o":"MyClass","x":58,"y":"value"}
```

This represents an instance of `MyClass` with `@x = 58` and `@y = "value"`.

### Circular References

When `:circular` option is enabled:

```json
{"^o":"MyClass","^i":1,"x":3,"me":"^r1"}
```

The `"^i":1` assigns ID 1 to this object, and `"^r1"` references it.

### Hash with Non-String Keys

Hash entries with non-String/Symbol keys are encoded as:

```json
{"^#3":[2,5]}
```

Where the value is `[key, value]` and `^#3` is a hex identifier.

### String Escaping

Ruby Strings starting with `':'` or `'^'` are escaped:
- `':'` becomes `'\u003a'`
- `'^'` becomes `'\u005e'`
