# mpact-sim Orchestration Guidelines

## System Rules & Architectural Guidelines

### Architectural Validation
* **Simulation Fidelity**: Systemic testing illusions mask architecture routing. Native execution MUST route authentic AST payloads and assert true decoding. Mocking `TargetEncoder`, `AxiSlave`, or asserting decoding with mocked `RealEncoding` instead of authentic illegal instruction trapping via ELF payloads completely evades actual integration boundaries and is strictly forbidden.

### Build Graph & Code Generation
* **Native Generator Fixes**: Fix the generator natively utilizing strict bitwise masking (`constexpr uint64_t kRiscv32InstLengthMask = 0x3`). Do not rely on `SafeEncodeWrapper` or `try...catch` abstractions.

### Orchestration Insights
* **Upstream Synchronization**: Continuous rebase syncs against `origin/main` are required to maintain a consistent orchestration ledger (`AGENTS.md`).
* **Namespace Shadowing & Emission Strictness**: Code generators must emit fully qualified global namespaces (e.g., `::absl::`, `::std::`) within their output strings. This prevents compilation failures caused by local namespace shadowing when generated headers are included inside specific internal namespaces like `mpact::sim`.
