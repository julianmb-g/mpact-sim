# mpact-sim Agent Instructions

## Lessons Learned

### Tier 1: Critical Constraints

- **Hermetic Build Preservation**
  * **Quote:** "Replacing `http_archive` with `local_repository` or `native.local_repository` in Bazel repository definitions is strictly forbidden."
  * **Impact:** Breaks hermeticity and cross-system reproducibility.
  * **Action:** Exclusively use `http_archive` for all Bazel dependencies.

- **Circular Dependency Prevention**
  * **Quote:** "Do not instruct generic toolkit submodules to implement integration tests that depend on downstream concrete implementations."
  * **Impact:** Introduces circular dependencies and breaks the Bazel build graph.
  * **Action:** Maintain strict boundary isolation; keep generic toolkit tests independent of downstream implementations.

- **Git Conflict Resolution Mandate**
  * **Quote:** "When resolving upstream rebase conflicts, never abort the rebase or revert using `git reset`."
  * **Impact:** Destroys the orchestration ledger and local state.
  * **Action:** Manually resolve conflicts by editing the file, staging with `git add`, and executing `git rebase --continue`.

- **Safe C++ API Boundaries**
  * **Quote:** "Natively generated `CoralnpuV2SlotMatcher::Encode()` APIs must utilize `absl::flat_hash_map::find()` rather than `.at(index)`."
  * **Impact:** Using `.at(index)` triggers C++ exceptions and `std::abort()` crashes on cache misses.
  * **Action:** Use `.find()` and gracefully return an `absl::NotFoundError` if an opcode string misses.

- **Native Generator Fixes**
  * **Quote:** "Implementing a generic `SafeEncodeWrapper` using `try...catch` violates the explicit directive to fix the generator natively."
  * **Impact:** Introduces unauthorized evasion abstractions instead of resolving root architectural flaws.
  * **Action:** Fix the generator natively. Do not reintroduce `SafeEncodeWrapper`.

- **Hardware Simulation & Mock Isolation Rules**
  * **Quote:** "Mocking AxiSlave with Python dictionaries and swallowing test exceptions."
  * **Impact:** Causes false-positive 100% unit tests that fail to simulate RTL component boundaries.
  * **Action:** Tests mimicking external memory MUST instantiate REAL synthesized DDR controllers and SRAM RTL block responders. Eviscerating memory boundaries to trap timeouts is strictly forbidden.

- **Authentic E2E Payload Routing**
  * **Quote:** "Validating bytes written to memory does not prove cross-component hardware integration. A mutator or wrapper component is invalid until an authentic AST payload routes natively."
  * **Impact:** Systemic testing illusions mask cross-component architectural routing and traps.
  * **Action:** MUST introduce rigorous Integration/E2E execution tests. Route authentic AST payloads (compiled ELFs) through the entire execution loop natively and verify traces. Mocking `TargetEncoder` is insufficient.

- **Eradication of Mock Decoding Illusions**
  * **Quote:** "Do not use hand-written mocks that blindly return canned scalar vectors... Do not leave trivial `EXPECT_TRUE(true)` placeholders."
  * **Impact:** Mathematically masks missing index bounds increments and skips underlying exceptions, constituting testing fraud.
  * **Action:** Natively implement structural bounds parsing, use `EXPECT_THROW` to organically test crashes, and assert true decoding correctness.

### Tier 2: System Architecture

- **Crash Evasion Boundary Alignment**
  * **Quote:** "Integration tests verifying the isolation boundary MUST explicitly accept both `kInternal` and `kNotFound` statuses rather than strictly expecting a 'crash' string."
  * **Impact:** Strictly expecting a "crash" string causes false-positive test failures when the decoder gracefully handles out-of-bounds indices.
  * **Action:** Update integration tests evaluating `AstEncoderSigsetjmpWorker` to explicitly accept `kNotFound` alongside `kInternal`.

- **Submodule Ledger Consolidation**
  * **Quote:** "Leaving 'Restored Knowledge' blocks at the bottom of the submodule AGENTS.md."
  * **Impact:** Fragments submodule-specific execution constraints and creates redundant duplication.
  * **Action:** Immediately integrate audit restorations into the primary strict execution mandates and remove the restoration headers. Prune exact duplicates.
- **EncodeSafe & Architectural Bitwise Masking Constraints**
  * **Quote:** "Refactor `EncodeSafe()` AST bitwise mutation logic... Replace raw literal masks (`inst |= 0x3`) with `kRiscv32InstLengthMask` and mask the lowest 2 bits."
  * **Impact:** Hardcoded literal masks (`0x3`) obscure architectural intent and increase the risk of bitwise regressions.
  * **Action:** Ensure the `EncodeSafe` backward-compatible C++ wrapper explicitly masks the lower 2 bits utilizing `constexpr uint64_t kRiscv32InstLengthMask = 0x3` to strictly preserve the 32-bit instruction length boundary.
