#include "mpact/sim/decoder/safe_encode_wrapper.h"
#include "absl/status/statusor.h"
#include "absl/status/status.h"
#include "gtest/gtest.h"
#include "mpact/sim/generic/instruction.h"
#include <exception>
#include <string>
#include <stdexcept>

namespace mpact {
namespace sim {
namespace decoder {
namespace {

struct TestPayload {
  std::string mnemonic;
};

class alignas(void*) TargetEncoder {
 public:
  absl::StatusOr<mpact::sim::generic::Instruction*> Encode(const TestPayload& payload) {
    if (payload.mnemonic == "INVALID_OUT_OF_RANGE") {
      throw std::out_of_range("AST constraint out of range");
    }
    if (payload.mnemonic == "UNMAPPED_OPCODE") {
      return absl::NotFoundError("Opcode not found in AST mapping");
    }
    return nullptr;
  }
};

TEST(SafeEncodeWrapperTest, HandlesSuccess) {
  TargetEncoder encoder;
  TestPayload payload{"VALID_OPCODE"};
  auto result = SafeEncodeWrapper<TargetEncoder, TestPayload>::EncodeSafe(&encoder, payload);
  EXPECT_TRUE(result.ok());
  EXPECT_EQ(result.value(), nullptr);
}

TEST(SafeEncodeWrapperTest, HandlesNotFound) {
  TargetEncoder encoder;
  TestPayload payload{"UNMAPPED_OPCODE"};
  auto result = SafeEncodeWrapper<TargetEncoder, TestPayload>::EncodeSafe(&encoder, payload);
  EXPECT_FALSE(result.ok());
  EXPECT_TRUE(absl::IsNotFound(result.status()));
}

TEST(SafeEncodeWrapperTest, HandlesOutOfRange) {
  TargetEncoder encoder;
  TestPayload payload{"INVALID_OUT_OF_RANGE"};
  auto result = SafeEncodeWrapper<TargetEncoder, TestPayload>::EncodeSafe(&encoder, payload);
  EXPECT_FALSE(result.ok());
  EXPECT_TRUE(absl::IsNotFound(result.status()));
}

TEST(SafeEncodeWrapperTest, MisalignedPointerOrganicBoundaryFailure) {
  TargetEncoder encoder;
  char* bad_ptr_base = reinterpret_cast<char*>(&encoder) + 1; // Misaligned
  TargetEncoder* bad_ptr = reinterpret_cast<TargetEncoder*>(bad_ptr_base);
  
  TestPayload payload{"VALID_OPCODE"};
  auto test_func = [bad_ptr, payload]() {
    auto result = SafeEncodeWrapper<TargetEncoder, TestPayload>::EncodeSafe(bad_ptr, payload);
    (void)result;
  };
  EXPECT_DEATH(test_func(), "");
}

}  // namespace
}  // namespace decoder
}  // namespace sim
}  // namespace mpact
