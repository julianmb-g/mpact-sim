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

namespace {

using namespace sim::example::isa;

class TestArchState : public mpact::sim::generic::ArchState {
 public:
  TestArchState() : mpact::sim::generic::ArchState("test") {}
};

class MockEncoding : public ExampleEncodingBase {
 public:
  OpcodeEnum GetOpcode(SlotEnum slot, int entry) override {
    return OpcodeEnum::kBrInd;
  }
};

TEST(CombinedDecoderTest, DecodeASide0) {
  TestArchState arch_state;
  MockEncoding encoding;
  ASide0Slot decoder(&arch_state);
  
  auto *inst = decoder.Decode(0x1000, &encoding, SlotEnum::kASide0, 0);
  EXPECT_NE(inst, nullptr);
  EXPECT_EQ(inst->address(), 0x1000);
  inst->DecRef();
}

}  // namespace
