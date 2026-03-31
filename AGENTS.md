# mpact-sim Orchestration Guidelines

## System Rules & Architectural Guidelines

### Architectural Validation
* **Exception Substitution Hazard**: Developers MUST explicitly evaluate `absl::StatusOr::ok()` before accessing `.value()`. Unconditional unwraps are strictly forbidden to prevent silent fuzzer terminations.
* **Simulation Fidelity**: Systemic testing illusions mask architecture routing. Native execution MUST route authentic AST payloads and assert true decoding. Mocking `TargetEncoder` or `AxiSlave` to test generic toolkit logic is strictly forbidden.

### Build Graph & Code Generation
* **Circular Dependencies**: Do not instruct generic toolkit submodules to implement integration tests that depend on downstream concrete implementations. Keep tests strictly independent.
* **Native Generator Fixes**: Fix the generator natively utilizing strict bitwise masking (`constexpr uint64_t kRiscv32InstLengthMask = 0x3`). Do not rely on `SafeEncodeWrapper` or `try...catch` abstractions.

### Orchestration Insights
* **Upstream Synchronization**: Continuous rebase syncs against `origin/main` are required to maintain a consistent orchestration ledger (`AGENTS.md`).
- **TargetEncoder Mocks**: MUST introduce rigorous Integration/E2E execution tests natively. Mocking TargetEncoder classes is strictly forbidden.
* **Masked Architectural Decode Errors**: Asserting `decoder.Decode() == nullptr` with a mocked `RealEncoding` instead of authentic illegal instruction trapping via ELF payload completely evades actual integration boundaries and is invalid testing.
