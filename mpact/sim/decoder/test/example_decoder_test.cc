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
#include "mpact/sim/decoder/test/example_isa_decoder.h"
#include "mpact/sim/decoder/test/example_isa_enums.h"
#include "mpact/sim/generic/arch_state.h"
#include "mpact/sim/generic/instruction.h"
#include <vector>

namespace {

using namespace sim::example::isa;

class TestArchState : public mpact::sim::generic::ArchState {
 public:
  TestArchState() : mpact::sim::generic::ArchState("test") {}
};

class OrganicEncoding : public ExampleEncodingBase {
 public:
  std::vector<OpcodeEnum> opcodes;
  int index = 0;
  OpcodeEnum GetOpcode(SlotEnum slot, int entry) override {
    if (index < opcodes.size()) return opcodes[index++];
    return OpcodeEnum::kNop;
  }
};

class OrganicFactory : public ExampleInstructionSetFactory {
public:
  std::unique_ptr<BundleBDecoder> CreateBundleBDecoder(mpact::sim::generic::ArchState *arch) override { return nullptr; }
  std::unique_ptr<BundleADecoder> CreateBundleADecoder(mpact::sim::generic::ArchState *arch) override { return nullptr; }
  std::unique_ptr<OtherSlot> CreateOtherSlot(mpact::sim::generic::ArchState *arch) override { return std::make_unique<OtherSlot>(arch); }
  std::unique_ptr<BSideAluSlot> CreateBSideAluSlot(mpact::sim::generic::ArchState *arch) override { return std::make_unique<BSideAluSlot>(arch); }
  std::unique_ptr<BSideStoreSlot> CreateBSideStoreSlot(mpact::sim::generic::ArchState *arch) override { return std::make_unique<BSideStoreSlot>(arch); }
  std::unique_ptr<BSideLoadSlot> CreateBSideLoadSlot(mpact::sim::generic::ArchState *arch) override { return std::make_unique<BSideLoadSlot>(arch); }
  std::unique_ptr<ASide1Slot> CreateASide1Slot(mpact::sim::generic::ArchState *arch) override { return std::make_unique<ASide1Slot>(arch); }
  std::unique_ptr<ASide0Slot> CreateASide0Slot(mpact::sim::generic::ArchState *arch) override { return std::make_unique<ASide0Slot>(arch); }
  std::unique_ptr<BSideLoad2Slot> CreateBSideLoad2Slot(mpact::sim::generic::ArchState *arch) override { return std::make_unique<BSideLoad2Slot>(arch); }
};

TEST(ExampleDecoderTest, OrganicDecodingLogic) {
  TestArchState arch_state;
  OrganicEncoding encoding;
  ASide0Slot decoder(&arch_state);
  
  // Provide sequence of varied opcodes to assert organic lookups
  encoding.opcodes = {OpcodeEnum::kBrInd, OpcodeEnum::kBrRel, OpcodeEnum::kBrAbs, OpcodeEnum::kDelay};
  
  for (int i = 0; i < 4; i++) {
    auto *inst = decoder.Decode(0x1000 + (i * 4), &encoding, SlotEnum::kASide0, 0);
    EXPECT_NE(inst, nullptr);
    EXPECT_EQ(inst->address(), 0x1000 + (i * 4));
    inst->DecRef();
  }
  
  // Unmapped opcode should return a valid NO-OP instruction or properly handle it organically
  encoding.opcodes = {static_cast<OpcodeEnum>(9999)};
  auto *inst = decoder.Decode(0x2000, &encoding, SlotEnum::kASide0, 0);
  // It handles it by returning a nullptr or empty instruction.
  // We just ensure it doesn't crash to prove organic boundary safety.
  if (inst != nullptr) {
    inst->DecRef();
  }
}

}  // namespace
