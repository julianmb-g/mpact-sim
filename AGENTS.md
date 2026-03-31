# mpact-sim Orchestration Guidelines

### Architectural Validation
* **Simulation Fidelity**: Systemic testing illusions mask architecture routing. Native execution MUST route authentic AST payloads and assert true decoding. Mocking `TargetEncoder`, `AxiSlave`, or asserting decoding with mocked `RealEncoding` instead of authentic illegal instruction trapping via ELF payloads completely evades actual integration boundaries and is strictly forbidden.

### Build Graph & Code Generation
* **Missing Test Integration File**: Do not delete test files (e.g., `decoder_trap_integration_test.cc`) from the filesystem without also removing them from the `BUILD` dependencies to prevent Bazel crashes.
* **Native Generator Fixes**: Fix generators natively using strict bitwise masking (e.g., `constexpr uint64_t kRiscv32InstLengthMask = 0x3`). Do not rely on `SafeEncodeWrapper` or `try...catch` abstractions.

### Orchestration Insights
* **Upstream Synchronization**: Continuous rebase syncs against `origin/main` are required to maintain a consistent orchestration ledger (`AGENTS.md`).
### Git & Orchestration Operations
* **Upstream Synchronization Strictness:** When checking for remote divergence or integrating new changes within this submodule, you MUST explicitly query the `upstream` tracking branch. You are STRICTLY FORBIDDEN from defaulting to `origin`. To view commits that exist upstream but are missing locally, use the exact command: `git log <local_branch>..upstream/<branch> --oneline` (Note: Use `master` for tinygrad, `main` for all others). NEVER run `git fetch` or `git pull` yourself; strictly rely on the pre-fetched `upstream/*` references.

### Git & Orchestration Operations
* **Upstream Synchronization Strictness:** When checking for remote divergence or integrating new changes within this submodule, you MUST explicitly query the `upstream` tracking branch. You are STRICTLY FORBIDDEN from defaulting to `origin`. To view commits that exist upstream but are missing locally, use the exact command: `git log <local_branch>..upstream/<branch> --oneline` (Note: Use `master` for tinygrad, `main` for all others). NEVER run `git fetch` or `git pull` yourself; strictly rely on the pre-fetched `upstream/*` references.
