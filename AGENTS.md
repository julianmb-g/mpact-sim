# mpact-sim Agent Instructions

## Lessons Learned

### Miscellaneous
- **C++ Scope Resolving (ELFIO)**: External libraries like `ELFIO` might update their namespacing (`ELFCLASS32` to `ELFIO::ELFCLASS32`). Explicit scope resolution is necessary during integration.
- **mpact-sim framework**: Contains core infrastructure like ISA/binary decoder generators, generic instruction and register classes, and utility models like ELF loaders.

### Testing Gotchas
- **Isolated Phase 2 Pointer Verification**: Even if a submodule pointer was previously synchronized and committed out-of-band by an overlapping cycle, the Build Agent must still execute the cross-submodule zero-trust verification (e.g., `bazel test //...`) mapped in Phase 2 before officially checking off the task in `PLAN.md` to guarantee structural integrity before advancing the SDLC loop. Note: When utilizing `--override_repository` for `mpact-sim` inside the `coralnpu-mpact` workspace, you must specify the full bazel repository name `--override_repository=com_google_mpact-sim=/workspace/louhi_ws/mpact-sim`, not just `mpact_sim`.
- **Coverage Masking via Trivial Assertions (EXPECT_TRUE)**: Unit tests for generated AST decoders MUST instantiate the generated decoders (e.g. `ASide0Slot`), pass a mock encoding interface (e.g. `MockEncoding`), and organically assert decoding correctness (`inst->address()`, `EXPECT_NE(inst, nullptr)`). Do not leave trivial `EXPECT_TRUE(true)` placeholders as they mathematically mask the entire AST generation logic and constitute testing fraud.
