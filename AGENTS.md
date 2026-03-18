# mpact-sim Agent Instructions

## Lessons Learned

### Architecture Quirks
- **mpact-sim framework**: Contains core infrastructure like ISA/binary decoder generators, generic instruction and register classes, and utility models like ELF loaders.

### Miscellaneous
- **C++ Scope Resolving (ELFIO)**: External libraries like `ELFIO` might update their namespacing (`ELFCLASS32` to `ELFIO::ELFCLASS32`). Explicit scope resolution is necessary during integration.
[FLAG: stale] - **Performance Over-Engineering (String Lookups)**: Avoid $O(N)$ string hashing in hot simulator paths (`Step()`). Cache `RegisterBase*` pointers during initialization.

[FLAG: stale] - **Exception Safety in Decoders**: Always use safe map accessors (`.find()`) instead of `.at()` when evaluating unknown AST inputs to prevent `std::out_of_range` exceptions from crashing simulator tools.
