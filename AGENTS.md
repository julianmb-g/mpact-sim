# mpact-sim Orchestration Guidelines

### Architectural Validation
* **Simulation Fidelity**: Systemic testing illusions mask architecture routing. Native execution MUST route authentic AST payloads and assert true decoding. Mocking `TargetEncoder`, `AxiSlave`, or asserting decoding with mocked `RealEncoding` instead of authentic illegal instruction trapping via ELF payloads completely evades actual integration boundaries and is strictly forbidden.
* **Testing Illusion Review**: Verify all bounds are strictly checked. No bypassing.

### Build Graph & Code Generation
* **Missing Test Integration File**: Do not delete test files (e.g., `decoder_trap_integration_test.cc`) from the filesystem without also removing them from the `BUILD` dependencies to prevent Bazel crashes.
* **Native Generator Fixes**: Fix generators natively using strict bitwise masking (e.g., `constexpr uint64_t kRiscv32InstLengthMask = 0x3`). Do not rely on `SafeEncodeWrapper` or `try...catch` abstractions.

### Orchestration Insights
* **JIT Sync Verification**: Upstream synchronization tasks MUST execute the specified verification tests (e.g., `bazel test //mpact/sim/decoder/...`) natively before completing the atomic cycle, even if the tracking branch is fully synchronized, to guarantee CI stability.
* **Upstream Synchronization Strictness**: JIT upstream synchronization operations must strictly verify `upstream/main` divergence atomically via `git log HEAD..upstream/main --oneline`. You are STRICTLY FORBIDDEN from defaulting to `origin` for these checks or running `git pull`/`git fetch`.

## Integration Execution Validation
* **Fake Integration Boundaries**: Do not use mocked `RealEncoding` or `TargetEncoder` classes for E2E Trap validation. Integration suites MUST natively route authentic executing ELF payloads through `riscv_top.cc`.
