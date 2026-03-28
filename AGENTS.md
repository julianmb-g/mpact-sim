# mpact-sim Agent Instructions

## Lessons Learned

### Tier 1: Critical Constraints & Blockers

- **Hermetic Build Preservation**
  - **Quote:** "Replacing `http_archive` with `local_repository` or `native.local_repository` in Bazel repository definitions (e.g., `repos.bzl`) is strictly forbidden across all submodules."
  - **Impact:** Doing so breaks hermeticity and cross-system reproducibility.
  - **Action:** Exclusively use `http_archive` for all Bazel dependencies.

- **Circular Dependency Prevention**
  - **Quote:** "Do not instruct generic toolkit submodules (like `mpact-sim`) to implement integration tests that depend on downstream concrete implementations (like `riscv_top.cc` in `mpact-riscv`)."
  - **Impact:** This introduces circular dependencies and breaks the Bazel build graph.
  - **Action:** Maintain strict boundary isolation; keep generic toolkit tests independent of downstream implementations.

- **Git Conflict Resolution Mandate**
  - **Quote:** "When resolving upstream rebase conflicts during a JIT Sync (e.g., `git rebase origin/main`), never abort the rebase or revert using `git reset`."
  - **Impact:** Using `git reset` or `git rebase --abort` destroys the orchestration ledger and local state.
  - **Action:** Manually resolve conflicts by editing the file, staging with `git add`, and executing `git rebase --continue`.

- **Safe C++ API Boundaries**
  - **Quote:** "Natively generated `CoralnpuV2SlotMatcher::Encode()` APIs must utilize `absl::flat_hash_map::find()` rather than `.at(index)`."
  - **Impact:** Using `.at(index)` triggers C++ exceptions and `std::abort()` crashes on cache misses.
  - **Action:** Use `.find()` and gracefully return an `absl::NotFoundError` if an opcode string misses.

- **Native Generator Fixes**
  - **Quote:** "Implementing a generic `SafeEncodeWrapper` using `try...catch` in `mpact-sim` violates the `SPECS.md` explicit directive to fix the generator natively."
  - **Impact:** Introduces unauthorized evasion abstractions instead of resolving root architectural flaws.
  - **Action:** Fix the generator natively. Do not reintroduce `SafeEncodeWrapper`.

- **Authentic E2E Payload Routing**
  - **Quote:** "A mutator or wrapper component is invalid until an authentic AST payload routes through the entire execution loop natively and verifies execution traces. Mocking `TargetEncoder` classes is insufficient."
  - **Impact:** Mocking integration boundaries mathematically masks cross-component architectural routing and traps.
  - **Action:** Introduce rigorous Integration/E2E execution tests using authentic AST payloads.

- **Eradication of Mock Decoding Illusions**
  - **Quote:** "Do not use hand-written mocks that blindly return canned scalar vectors... Do not leave trivial `EXPECT_TRUE(true)` placeholders."
  - **Impact:** Fake implementations and trivial assertions mathematically mask missing index bounds increments and skip underlying exceptions, constituting testing fraud.
  - **Action:** Natively implement structural bounds parsing, use `EXPECT_THROW` to organically test crashes, and assert true decoding correctness (e.g., `EXPECT_NE(inst, nullptr)`).

### Tier 2: Maintainability & Test Tuning

- **Crash Evasion Boundary Alignment**
  - **Quote:** "Integration tests verifying the isolation boundary MUST explicitly accept both `kInternal` and `kNotFound` statuses rather than strictly expecting a 'crash' string or internal code."
  - **Impact:** Strictly expecting a "crash" string causes false-positive test failures when the decoder gracefully handles out-of-bounds indices.
  - **Action:** Update integration tests evaluating `AstEncoderSigsetjmpWorker` to explicitly accept `kNotFound` alongside `kInternal`.
[FLAG: stale] ### Restored Knowledge (Data-Loss Audit Remediation - Testing Illusion Mandates)
- **Tier 1: Hardware Simulation & Mock Isolation Rules**
  - **Quote:** "Mocking AxiSlave with Python dictionaries and swallowing test exceptions."
  - **Impact:** False positive 100% unit tests that fail to simulate RTL component boundaries.
  - **Action:** tests mimicking external memory MUST instantiate REAL synthesized DDR controllers and SRAM RTL block responders. Eviscerating memory boundaries to trap timeouts is strictly forbidden.
- **Tier 1: E2E Integration Boundary Rules**
  - **Quote:** "Validating bytes written to memory does not prove cross-component hardware integration."
  - **Impact:** Systemic testing illusions where isolated string matching or individual instruction evaluations bypass cross-component routing.
  - **Action:** MUST introduce rigorous Integration/E2E execution tests. A mutator or wrapper component is invalid until an authentic AST payload (compiled ELF) routes through the entire execution loop natively and verifies execution traces. Mocking TargetEncoder classes is insufficient.

### Tier 2: Documentation & Ledger Maintenance
* **[Tier 2] Submodule Ledger Consolidation**
  * **Quote**: "Leaving 'Restored Knowledge' blocks at the bottom of the submodule AGENTS.md."
  * **Impact**: Fragments submodule-specific execution constraints.
  * **Action**: Immediately integrate audit restorations into the primary strict execution mandates and remove the restoration headers.
