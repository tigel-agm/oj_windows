# Oj::Parser API

The `Oj::Parser` API provides high-performance JSON parsing with isolated configuration.

## Overview

Unlike `Oj.load()` which uses global default options, `Oj::Parser` allows creating
separate parser instances, each with its own configuration. This provides:

- **No option leakage** between different use cases
- **Better performance** by avoiding per-call option processing
- **Cleaner API** with method-based option setting

## Creating Parsers

```ruby
require 'oj_windows'

# Validation parser
validator = Oj::Parser.new(:validate)

# Callback parser (SAJ-style)
saj_parser = Oj::Parser.new(:saj)

# Object builder parser
usual_parser = Oj::Parser.new(:usual)
```

## Parser Types

### :validate

Validates JSON without building Ruby objects. Fastest option when you only need
to check if JSON is well-formed.

```ruby
parser = Oj::Parser.new(:validate)
parser.parse('{"valid": true}')  # Returns nil, no exception = valid
```

### :saj (Callback)

Provides callbacks for each JSON element. Compatible with `Oj::Saj` handlers.

```ruby
class MyHandler < Oj::Saj
  def hash_start(key); end
  def hash_end(key); end
  def array_start(key); end
  def array_end(key); end
  def add_value(value, key); end
end

parser = Oj::Parser.new(:saj)
parser.handler = MyHandler.new
parser.parse(json_string)
```

### :usual

Builds Ruby objects from JSON. The most common parser type.

```ruby
parser = Oj::Parser.new(:usual)
parser.symbol_keys = true
parser.cache_keys = true

data = parser.parse('{"name": "test"}')
# => {:name => "test"}
```

## Configuration Options

Set options as methods on the parser instance:

```ruby
parser = Oj::Parser.new(:usual)
parser.symbol_keys = true      # Use Symbol keys instead of String
parser.cache_keys = true       # Cache frequently used keys
parser.cache_strings = 6       # Cache strings up to length 6
parser.create_id = '^o'        # Object creation identifier
```

## Performance

Benchmarks on Windows with Ruby 3.4.8 MSVC (50,000 iterations):

### Validation
```
             System  time (secs)  rate (ops/sec)
-------------------  -----------  --------------
       Oj::Saj.none       0.087      575904.833
Oj::Parser.validate       0.093      535818.388
```

### Callback (SAJ)
```
        System  time (secs)  rate (ops/sec)
--------------  -----------  --------------
Oj::Parser.saj       0.623       80316.627
   Oj::Saj.all       0.893       55984.415
```

`Oj::Parser.new(:saj)` is **1.43x** faster than `Oj.saj_parse`.

### Parse to Primitives
```
          System  time (secs)  rate (ops/sec)
----------------  -----------  --------------
Oj::Parser.usual       0.329      152133.829
       JSON::Ext       0.337      148264.387
 Oj::strict_load       0.528       94769.267
```

`Oj::Parser.new(:usual)` is **1.03x** faster than JSON::Ext.

### Object Parsing
```
          System  time (secs)  rate (ops/sec)
----------------  -----------  --------------
Oj::Parser.usual       0.054      921939.392
       JSON::Ext       0.076      656601.405
 Oj::compat_load       0.142      352839.974
```

`Oj::Parser.new(:usual)` is **1.40x** faster than JSON::Ext and
**2.61x** faster than `Oj.compat_load`.

## Summary

On Windows MSVC Ruby 3.4.8, `Oj::Parser` provides:
- Up to **2.6x faster** parsing than legacy Oj methods
- Isolated configuration per parser instance
- Better memory efficiency through caching
