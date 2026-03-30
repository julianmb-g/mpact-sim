# mpact-sim Orchestration Guidelines

## System Rules & Architectural Guidelines

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

* **Exception Substitution Hazard (`absl::StatusOr`)**
  * **Quote:** "Unconditional `.value()` unwrapping throws `absl::BadStatusOrAccess`, instantly terminating the fuzzer natively in-process."
  * **Impact:** Masks silent architectural panics or breaks downstream ABI integration points via undefined behaviors.
  * **Action:** Developers MUST explicitly evaluate `.ok()` before accessing `.value()`. Unconditional unwraps are strictly forbidden.

### Code Generation & Boundaries

* **Namespace Shadowing (`absl::string_view`)**
  * **Quote:** "Namespace shadowing inside nested generator blocks."
  * **Impact:** Compilation errors when generating C++ code that references external namespaces inside local scopes.
  * **Action:** When generating C++ code referencing external namespaces (like `absl`), ALWAYS use fully qualified global prefixes (e.g., `::absl::string_view`, `::absl`) to prevent namespace shadowing.
* **Operand Encoding Bounds Check Exception**
  * **Quote:** "The auto-generated bitwise packing logic may throw `std::out_of_range` if an operand immediate value exceeds its bit-width."
  * **Impact:** Exceptions fatally crash the process.
  * **Action:** Patch the generator to perform safe bounds-checking on operands and return an `absl::Status` (e.g., `absl::NotFoundError` or `absl::OutOfRangeError`) instead of throwing exceptions. Ensure comprehensive testing organically validates this without downstream circular dependencies.

### Orchestration Insights

* **Upstream Synchronization**
  * **Action:** Continuous rebase syncs against `origin/main` are required to maintain a consistent orchestration ledger (`AGENTS.md`).
* **Submodule Ledger Consolidation**
  * **Action:** Immediately integrate audit restorations into the primary strict execution mandates and remove restoration headers. Prune exact duplicates.