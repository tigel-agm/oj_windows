# Advanced Features

`oj_windows` is a high-performance JSON parser and serializer for Windows MSVC Ruby.
It provides multiple serialization modes and alternative parsing APIs for different use cases.

## Serialization Modes

`oj_windows` has several `dump` modes which control how Ruby Objects are
converted to JSON. These modes are set with the `:mode` option in either the
default options or as one of the options to the `dump` method:

- `:strict` - Strict JSON compliance
- `:compat` - JSON gem compatibility
- `:object` - Full Ruby object serialization
- `:custom` - Customizable behavior
- `:rails` - Rails/ActiveSupport integration
- `:wab` - WAB format

## Alternative Parser APIs

### Oj::Doc API

The fastest alternative parser API is the `Oj::Doc` API. It takes
a completely different approach by opening a JSON document and providing calls
to navigate around the JSON while it is open. This approach provides
efficient access for large documents where only specific elements are needed.

### Callback Parsers (SAJ/SCP)

The `Oj::Saj` and `Oj::ScHandler` APIs are callback parsers that
walk the JSON document depth first and make callbacks for each element.
Both callback parsers are useful when only portions of the JSON are of
interest, providing memory-efficient parsing of large documents.

### Oj::Parser API

The `Oj::Parser` API provides the best balance of performance and flexibility.
See [Parser.md](Parser.md) for detailed benchmarks on Windows MSVC Ruby 3.4.8.
