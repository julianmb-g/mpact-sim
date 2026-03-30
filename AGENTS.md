## Lessons Learned & Orchestration Rules

### Tier 1: Critical Blocker

* **Anti-Mocking & Simulation Fidelity**
  * **Quote:** "Validating bytes written to memory does not prove cross-component hardware integration. Do not leave trivial `EXPECT_TRUE(true)` placeholders."
  * **Impact:** Systemic testing illusions and mathematical evasion mask cross-component architectural routing and traps.
  * **Action:** E2E execution tests MUST natively route authentic AST payloads (compiled ELFs). Tests mimicking external memory MUST instantiate REAL synthesized DDR controllers and SRAM RTL block responders. Natively implement structural bounds parsing, use `EXPECT_THROW` to organically test crashes, and assert true decoding correctness. Mocking `TargetEncoder` or `AxiSlave` is strictly forbidden.

* **Circular Dependency Prevention**
  * **Quote:** "Do not instruct generic toolkit submodules to implement integration tests that depend on downstream concrete implementations."
  * **Impact:** Introduces circular dependencies and breaks the Bazel build graph.
  * **Action:** Maintain strict boundary isolation; keep generic toolkit tests independent of downstream implementations.

* **Hermetic Build Preservation**
  * **Quote:** "Replacing `http_archive` with `local_repository` or `native.local_repository` in Bazel repository definitions is strictly forbidden."
  * **Impact:** Breaks hermeticity and cross-system reproducibility.
  * **Action:** Exclusively use `http_archive` for all Bazel dependencies.

* **Native Generator Fixes & Wrapper Bans**
  * **Quote:** "Implementing a generic `SafeEncodeWrapper` using `try...catch` violates the explicit directive to fix the generator natively."
  * **Impact:** Introduces unauthorized evasion abstractions instead of resolving root architectural flaws.
  * **Action:** Fix the generator natively utilizing strict bitwise masking (`constexpr uint64_t kRiscv32InstLengthMask = 0x3`). Do not reintroduce `SafeEncodeWrapper` or evasion wrappers.

### Tier 2: System Architecture

* **Exception Substitution Hazard (absl::StatusOr)**
  * **Quote:** "Unconditional `.value()` unwrapping throws `absl::BadStatusOrAccess`, instantly terminating the fuzzer natively in-process."
  * **Impact:** Masks silent architectural panics or breaks downstream ABI integration points via undefined behaviors.
  * **Action:** Developers MUST explicitly evaluate `.ok()` before accessing `.value()`. Unconditional unwraps are strictly forbidden.

### Tier 2: System Architecture & Clarification Needed

* **Submodule Ledger Consolidation**
  * **Quote:** "Leaving 'Restored Knowledge' blocks at the bottom of the submodule AGENTS.md."
  * **Impact:** Fragments submodule-specific execution constraints and creates redundant duplication.
  * **Action:** Immediately integrate audit restorations into the primary strict execution mandates and remove the restoration headers. Prune exact duplicates.

# mpact-sim Orchestration Guidelines

### Orchestration Execution Insights (Cycle 165 - mpact-sim Sync)

* **Upstream Synchronization**: Continuous rebase syncs against `origin/main` are required to maintain a consistent orchestration ledger (`AGENTS.md`).

### Globally Relevant Execution Rules (Appended)

### Orchestration Execution Insights (Cycle 167)
* **Operand Encoding Bounds Check Exception**: The auto-generated bitwise packing logic may throw `std::out_of_range` if an operand immediate value exceeds its bit-width. Patch the generator to perform safe bounds-checking on operands and return an `absl::Status` (e.g., `absl::NotFoundError`) instead of throwing exceptions.

### Orchestration Execution Insights (Cycle 167 - Operand Testing)
* **Operand Encoding Bounds Comprehensive Test**: Ensure we have tests that comprehensively validate the new auto-generated AST encoder patch that returns `absl::OutOfRangeError` on out-of-bounds operand values, rather than crashing. These tests should cover bounds checking organically without introducing circular dependencies to downstream concrete implementations.

### Orchestration Execution Insights (Cycle 167 - QA Audit)

* **Namespace Shadowing (`absl::string_view`)**: The instruction decoder generator MUST use fully qualified global prefixes (e.g., `::absl::string_view`) when referencing external namespaces inside the local `mpact::sim::decoder` scope to prevent namespace shadowing compilation errors.
