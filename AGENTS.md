# mpact-sim Agent Instructions

## Lessons Learned

### Tier 1: Critical Constraints

* **Hermetic Build Preservation**
  * **Quote:** "Replacing `http_archive` with `local_repository` or `native.local_repository` in Bazel repository definitions is strictly forbidden."
  * **Impact:** Breaks hermeticity and cross-system reproducibility.
  * **Action:** Exclusively use `http_archive` for all Bazel dependencies.

* **Circular Dependency Prevention**
  * **Quote:** "Do not instruct generic toolkit submodules to implement integration tests that depend on downstream concrete implementations."
  * **Impact:** Introduces circular dependencies and breaks the Bazel build graph.
  * **Action:** Maintain strict boundary isolation; keep generic toolkit tests independent of downstream implementations.

* [FLAG: stale] **Git Conflict Resolution Mandate**
  * **Quote:** "When resolving upstream rebase conflicts, never abort the rebase or revert using `git reset`."
  * **Impact:** Destroys the orchestration ledger and local state.
  * **Action:** [FLAG: stale] Manually resolve conflicts by editing the file, staging with `git add`, and executing `git rebase --continue`.

* **Safe C++ API Boundaries**
  * **Quote:** "Natively generated `CoralnpuV2SlotMatcher::Encode()` APIs must utilize `absl::flat_hash_map::find()` rather than `.at(index)`."
  * **Impact:** Using `.at(index)` triggers C++ exceptions and `std::abort()` crashes on cache misses.
  * **Action:** Use `.find()` and gracefully return an `absl::NotFoundError` if an opcode string misses.

* **Native Generator Fixes & Wrapper Bans**
  * **Quote:** "Implementing a generic `SafeEncodeWrapper` using `try...catch` violates the explicit directive to fix the generator natively."
  * **Impact:** Introduces unauthorized evasion abstractions instead of resolving root architectural flaws.
  * **Action:** Fix the generator natively utilizing strict bitwise masking (`constexpr uint64_t kRiscv32InstLengthMask = 0x3`). Do not reintroduce `SafeEncodeWrapper` or evasion wrappers.

* **Anti-Mocking & Simulation Fidelity**
  * **Quote:** "Validating bytes written to memory does not prove cross-component hardware integration. Do not leave trivial `EXPECT_TRUE(true)` placeholders."
  * **Impact:** Systemic testing illusions and mathematical evasion mask cross-component architectural routing and traps.
  * **Action:** E2E execution tests MUST natively route authentic AST payloads (compiled ELFs). Tests mimicking external memory MUST instantiate REAL synthesized DDR controllers and SRAM RTL block responders. Natively implement structural bounds parsing, use `EXPECT_THROW` to organically test crashes, and assert true decoding correctness. Mocking `TargetEncoder` or `AxiSlave` is strictly forbidden.

### Tier 2: System Architecture

* [FLAG: invalid] **Crash Evasion Boundary Alignment**
  * **Quote:** "Integration tests verifying the isolation boundary MUST explicitly accept both `kInternal` and `kNotFound` statuses rather than strictly expecting a 'crash' string."
  * **Impact:** Strictly expecting a "crash" string causes false-positive test failures when the decoder gracefully handles out-of-bounds indices.
  * **Action:** [FLAG: invalid] Update integration tests evaluating `AstEncoderSigsetjmpWorker` to explicitly accept `kNotFound` alongside `kInternal`.

* **Submodule Ledger Consolidation**
  * **Quote:** "Leaving 'Restored Knowledge' blocks at the bottom of the submodule AGENTS.md."
  * **Impact:** Fragments submodule-specific execution constraints and creates redundant duplication.
  * **Action:** Immediately integrate audit restorations into the primary strict execution mandates and remove the restoration headers. Prune exact duplicates.

# mpact-sim Orchestration Guidelines

* **Exception Substitution Hazard (absl::StatusOr)**
  * **Quote:** "Unconditional `.value()` unwrapping throws `absl::BadStatusOrAccess`, instantly terminating the fuzzer natively in-process."
  * **Impact:** Masks silent architectural panics or breaks downstream ABI integration points via undefined behaviors.
  * **Action:** Developers MUST explicitly evaluate `.ok()` before accessing `.value()`. Unconditional unwraps are strictly forbidden.
