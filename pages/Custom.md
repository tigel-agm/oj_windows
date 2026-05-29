# Custom Mode

The `:custom` mode is the most configurable mode in `oj_windows` and honors almost all
options. It provides maximum flexibility for JSON serialization and parsing.

## Overview

The `:custom` mode can be configured by:
- Passing options to `Oj.dump()` and `Oj.load()` methods
- Modifying the default options via `Oj.default_options`

## Object Creation

The ability to create Ruby objects from JSON object elements is supported via
the `:create_additions` option.

Special treatment is given to the `:create_id` option:

| `:create_id` Value | Behavior |
|--------------------|----------|
| `"^o"` | Uses efficient internal encoding/decoding (recommended) |
| Other values | Calls `to_json` and `create_json` methods (JSON gem compatible) |

When using `"^o"`, the internal encoding is more efficient and does not require
the target classes to implement `to_json` or `create_json` methods.

## Configuration Example

```ruby
require 'oj_windows'

Oj.default_options = {
  mode: :custom,
  create_additions: true,
  create_id: '^o'
}
```
