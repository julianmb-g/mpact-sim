
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

* **Safe C++ API Boundaries**
  * **Quote:** "Natively generated `CoralnpuV2SlotMatcher::Encode()` APIs must utilize `absl::flat_hash_map::find()` rather than `.at(index)`."
  * **Impact:** Using `.at(index)` triggers C++ exceptions and `std::abort()` crashes on cache misses.
  * **Action:** Use `.find()` and gracefully return an `absl::NotFoundError` if an opcode string misses.

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

* **Dangerous Commands & Missing Teardown (Bazel Deadlocks)**
  * **Quote:** "Launching massive monolithic Bazel test suites concurrently without pkill -f bazel or resource constraints guarantees CI orchestrator timeouts."
  * **Impact:** CI timeout due to Bazel server memory exhaustion.
  * **Action:** Prepend and append `pkill -f "bazel" || true` to all Bazel batch executions. Apply strict `--local_resources=cpu=8 --local_resources=memory=HOST_RAM*0.5` flags.

* **Toolchain Isolation & Hermetic Boundaries**
  * **Quote:** "The RISC-V cross-compilers... must be installed on the host machine. Hermetic Build Exemption..."
  * **Impact:** Autonomous agents will pollute the hermetic Bazel workspace with host toolchains if not explicitly bounded.
  * **Action:** Agents must strictly enforce Cross-Compiler Testing Matrix & Toolchain Isolation. Exclusively `tinygrad` is allowed to use host cross-compilers; all other Bazel submodules must use strict hermetic toolchains.

* **Backward Compatibility & Crash Tracing**
  * **Quote:** "Abstracting root cause locations hides the source of crashes. Legacy keyword arguments must be strictly preserved via `**kwargs`."
  * **Impact:** Causes cascading API contract breakages and obscures exact component failures.
  * **Action:** Explicitly fix files instead of relying on broad mocking. Ensure safe fallbacks via `**kwargs` when refactoring core APIs.
