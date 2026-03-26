# mpact-sim Agent Instructions
## Lessons Learned

### Build & Orchestration
- **Flawed Execution Boundaries (Submodule Scope)**: Do not instruct generic toolkit submodules (like `mpact-sim`) to implement integration tests that depend on downstream concrete implementations (like `riscv_top.cc` in `mpact-riscv`). This introduces circular dependencies and breaks the Bazel build graph.
- **Upstream Sync & Rebase Conflicts**: When resolving upstream rebase conflicts during a JIT Sync (e.g., `git rebase origin/main`), never abort the rebase or revert using `git reset`. Manually resolve conflicts by editing the file and running `git rebase --continue`.

### C++ & System Programming
- **Encode() Safe Fallbacks**: Natively generated `CoralnpuV2SlotMatcher::Encode()` APIs must utilize `absl::flat_hash_map::find()` rather than `.at(index)`. If a generated opcode string misses, it must gracefully return an `absl::NotFoundError` to prevent C++ exceptions and `std::abort()` crashes.
- **Test Integrity (Crash Evasion Boundaries)**: When writing tests targeting crash recovery via `AstEncoderSigsetjmpWorker`, remember that `mpact-sim`'s decoder `Encode()` gracefully handles index out-of-bounds by returning `absl::StatusCode::kNotFound` instead of crashing via `std::abort()`. Integration tests verifying the isolation boundary MUST explicitly accept both `kInternal` and `kNotFound` statuses rather than strictly expecting a "crash" string or internal code, avoiding false-positive test failures.

### QA & Testing Gotchas
- **Coverage Masking via Trivial Assertions (EXPECT_TRUE)**: Unit tests for generated AST decoders MUST instantiate the generated decoders (e.g. `ASide0Slot`), pass a mock encoding interface (e.g. `MockEncoding`), and organically assert decoding correctness (`inst->address()`, `EXPECT_NE(inst, nullptr)`). Do not leave trivial `EXPECT_TRUE(true)` placeholders as they mathematically mask the entire AST generation logic and constitute testing fraud.
- **Eradication of Mock Decoding Illusions**: Do not use hand-written mocks that blindly return canned scalar vectors (e.g. `opcodes[index++]`) when testing ISA decoder boundaries (like `ASide0Slot::Decode`). These fake implementations mathematically mask missing index bounds increments and skip underlying exceptions (like `std::out_of_range` on unmapped `GetOpcode` bits). Natively implement structural bounds parsing (e.g. `inst_word_ & 0xFFFF`) and use `EXPECT_THROW` to organically test execution crashes instead of relying on the mock illusion.

- **TargetEncoder Mocking Limits**: `mpact-sim` MUST introduce rigorous Integration/E2E execution tests. A mutator or wrapper component is invalid until an authentic AST payload routes through the entire execution loop natively and verifies execution traces. Mocking `TargetEncoder` classes is insufficient.
