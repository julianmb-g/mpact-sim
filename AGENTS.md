# mpact-sim Agent Instructions

## Lessons Learned

### Architectural Ledger and Data Loss Preservation
- **Eradication of Mock Decoding Illusions**: Do not use hand-written mocks that blindly return canned scalar vectors (e.g. `opcodes[index++]`) when testing ISA decoder boundaries (like `ASide0Slot::Decode`). These fake implementations mathematically mask missing index bounds increments and skip underlying exceptions (like `std::out_of_range` on unmapped `GetOpcode` bits). Natively implement structural bounds parsing (e.g. `inst_word_ & 0xFFFF`) and use `EXPECT_THROW` to organically test execution crashes instead of relying on the mock illusion.

### Miscellaneous
- **mpact-sim framework**: Contains core infrastructure like ISA/binary decoder generators, generic instruction and register classes, and utility models like ELF loaders.

### Testing Gotchas
- **C++ Scope Resolving (ELFIO)**: External libraries like `ELFIO` might update their namespacing (`ELFCLASS32` to `ELFIO::ELFCLASS32`). Explicit scope resolution is necessary during integration.
- **C++ Variable Naming Code Smells (auto)**: Avoid ambiguous variable naming with `auto` when receiving `absl::Status` or `absl::StatusOr` return values (e.g., avoid `auto res = worker->Encode()`). Use explicit types like `absl::StatusOr<InstructionGroup*> result =` to clarify the return type and prevent aggressive context-switching during incident response error handling.
- **Coverage Masking via Trivial Assertions (EXPECT_TRUE)**: Unit tests for generated AST decoders MUST instantiate the generated decoders (e.g. `ASide0Slot`), pass a mock encoding interface (e.g. `MockEncoding`), and organically assert decoding correctness (`inst->address()`, `EXPECT_NE(inst, nullptr)`). Do not leave trivial `EXPECT_TRUE(true)` placeholders as they mathematically mask the entire AST generation logic and constitute testing fraud.
- **Generated Code Hallucination**: When a task requests editing a file like `decoder/slot_matcher.h` which does not statically exist in the repository but is generated dynamically during the build (e.g., via `instruction_set.cc`), agents MUST modify the actual generator source file, not attempt to create or edit the hallucinated generated artifact.
- **Isolated Phase 2 Pointer Verification**: Even if a submodule pointer was previously synchronized and committed out-of-band by an overlapping cycle, the Build Agent must still execute the cross-submodule zero-trust verification (e.g., `bazel test //...`) mapped in Phase 2 before officially checking off the task in `PLAN.md` to guarantee structural integrity before advancing the SDLC loop. Note: When utilizing `--override_repository` for `mpact-sim` inside the `coralnpu-mpact` workspace, you must specify the full bazel repository name `--override_repository=com_google_mpact-sim=/workspace/louhi_ws/mpact-sim`, not just `mpact_sim`.
- **JIT Sync Output Aggregation**: When verifying zero divergence across multiple clean submodules simultaneously, ensure that the verification note `(Verified: zero-divergence, empty stash, bypassed redundant tests)` is strictly appended to every checklist item to maintain adversarial ledger fidelity.
- **Simulated Code Constraints**: Keep generated AST decoders aligned with the root architecture specs.
- **absl::StatusOr Negation Syntax Error**: The unary `-` operator cannot be applied directly to an `absl::StatusOr<T>` wrapper in C++. Developers must explicitly evaluate `.ok()` and unwrap `.value()` before negating to prevent fatal `FAILED TO BUILD` pipeline crashes. Do not implement static `operator-` overloads for `absl::StatusOr` to falsely mask this error.
