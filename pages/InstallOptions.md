# Oj Windows Install Options

### Enable trace log

```powershell
gem install oj_windows -- --enable-trace-log
```

To enable Oj trace feature on Windows, use the `--enable-trace-log` option when installing the gem.
Then, the trace logs will be displayed when `:trace` option is set to `true`.


### SIMD string scanning

SIMD acceleration of string scanning is **enabled by default** on x86_64 (SSE2,
which every x64 CPU supports) — no flag is required. The parser selects its
scanner at load time; by default it uses the SSE2 scanner, which benchmarks
fastest on common CPUs.

To force the portable scalar scanner (e.g. for debugging or on a CPU where you
want to compare), build with `--disable-simd`:

```powershell
gem install oj_windows -- --disable-simd
```

You can also override the scanner at runtime, without rebuilding, via the
`OJ_SCAN` environment variable: `scalar`, `sse2`, or `sse42`. For example
`set OJ_SCAN=sse42` opts into the SSE4.2 (PCMPESTRI) scanner, which can be
faster on some microarchitectures.
