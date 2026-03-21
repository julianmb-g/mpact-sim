# mpact-sim Agent Instructions

## Lessons Learned


### Miscellaneous
- **C++ Scope Resolving (ELFIO)**: External libraries like `ELFIO` might update their namespacing (`ELFCLASS32` to `ELFIO::ELFCLASS32`). Explicit scope resolution is necessary during integration.
- **mpact-sim framework**: Contains core infrastructure like ISA/binary decoder generators, generic instruction and register classes, and utility models like ELF loaders.

### Testing Gotchas
- **Isolated Phase 2 Pointer Verification**: Even if a submodule pointer was previously synchronized and committed out-of-band by an overlapping cycle, the Build Agent must still execute the cross-submodule zero-trust verification (e.g., `bazel test //...`) mapped in Phase 2 before officially checking off the task in `PLAN.md` to guarantee structural integrity before advancing the SDLC loop. Note: When utilizing `--override_repository` for `mpact-sim` inside the `coralnpu-mpact` workspace, you must specify the full bazel repository name `--override_repository=com_google_mpact-sim=/workspace/louhi_ws/mpact-sim`, not just `mpact_sim`.
