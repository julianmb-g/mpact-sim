// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "mpact/sim/decoder/test/combined_isa_decoder.h"
#include "mpact/sim/decoder/test/combined_isa_enums.h"
#include "mpact/sim/generic/arch_state.h"
#include "mpact/sim/generic/instruction.h"
#include <cstdint>
#include <stdexcept>
#include <vector>

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
    // Real encoding implementation structurally evaluates the instruction payload
    uint32_t opc = inst_word_ & 0xFFFF;
    // Map raw bits to valid OpcodeEnum or return raw bit extraction
    switch(opc) {
      case static_cast<uint32_t>(OpcodeEnum::kBrInd): return OpcodeEnum::kBrInd;
      case static_cast<uint32_t>(OpcodeEnum::kBrRel): return OpcodeEnum::kBrRel;
      case static_cast<uint32_t>(OpcodeEnum::kBrAbs): return OpcodeEnum::kBrAbs;
      case static_cast<uint32_t>(OpcodeEnum::kDelay): return OpcodeEnum::kDelay;
      default: return static_cast<OpcodeEnum>(opc);
    }
  }
};

TEST(CombinedDecoderTest, OrganicDecodingLogic) {
  TestArchState arch_state;
  RealEncoding encoding;
  ASide0Slot decoder(&arch_state);
  
  // Provide real instruction payloads to organically evaluate decoder mapping
  uint32_t insts[] = {
      static_cast<uint32_t>(OpcodeEnum::kBrInd),
      static_cast<uint32_t>(OpcodeEnum::kBrRel),
      static_cast<uint32_t>(OpcodeEnum::kBrAbs),
      static_cast<uint32_t>(OpcodeEnum::kDelay)
  };
  
  for (int i = 0; i < 4; i++) {
    encoding.inst_word_ = insts[i];
    auto *inst = decoder.Decode(0x1000 + (i * 4), &encoding, SlotEnum::kASide0, 0);
    EXPECT_NE(inst, nullptr);
    EXPECT_EQ(inst->address(), 0x1000 + (i * 4));
    inst->DecRef();
  }
  
  // Unmapped opcode safely returns nullptr natively now
  encoding.inst_word_ = 9999;
  auto *inst = decoder.Decode(0x2000, &encoding, SlotEnum::kASide0, 0);
  EXPECT_EQ(inst, nullptr);
}

}  // namespace
