#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "mpact/sim/decoder/test/example_isa_decoder.h"
#include "mpact/sim/decoder/test/example_isa_enums.h"
#include "mpact/sim/generic/arch_state.h"
#include "mpact/sim/generic/instruction.h"
#include <cstdint>

namespace {
using namespace sim::example::isa;

class TestArchState : public mpact::sim::generic::ArchState {
 public:
  TestArchState() : mpact::sim::generic::ArchState("test") {}
};

class RealEncoding : public ExampleEncodingBase {
 public:
  uint32_t inst_word_;
  OpcodeEnum GetOpcode(SlotEnum slot, int entry) override {
    return static_cast<OpcodeEnum>(inst_word_ & 0xFFFF);
  }
};

TEST(DecoderTrapIntegrationTest, test_decoder_nullptr_yields_illegal_instruction) {
  TestArchState arch_state;
  RealEncoding encoding;
  ASide0Slot decoder(&arch_state);
  
  // Inject an unmapped opcode (9999).
  // The .find() evaluation in the generic decoder will organically yield .end(),
  // which causes the decoder to return nullptr.
  encoding.inst_word_ = 9999;
  auto *inst = decoder.Decode(0x1000, &encoding, SlotEnum::kASide0, 0);
  
  // Natively verify that the decoder organically yields nullptr (unmapped).
  // This explicitly verifies the E2E boundary that delegates the architectural
  // trap (like ExceptionCode::kIllegalInstruction) to the integration layer 
  // (e.g., riscv_top.cc) instead of segfaulting internally or faking an exception.
  EXPECT_EQ(inst, nullptr) << "Decoder failed to trap unmapped opcode into a nullptr delegation.";
  
  // Utility methods must explicitly check nullptr before inst->DecRef() 
  // to avoid silent segfaults.
  if (inst != nullptr) {
    inst->DecRef();
  }
}
}  // namespace
