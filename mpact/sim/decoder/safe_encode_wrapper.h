#ifndef MPACT_SIM_DECODER_SAFE_ENCODE_WRAPPER_H_
#define MPACT_SIM_DECODER_SAFE_ENCODE_WRAPPER_H_

#include "absl/status/statusor.h"
#include "absl/status/status.h"
#include "absl/log/check.h"
#include "mpact/sim/generic/instruction.h"
#include <memory>
#include <stdexcept>

namespace mpact {
namespace sim {
namespace decoder {

template <typename Encoder, typename Payload>
class SafeEncodeWrapper {
 public:
  static absl::StatusOr<mpact::sim::generic::Instruction*> EncodeSafe(Encoder* encoder, const Payload& payload) {
    if (!encoder) {
      return absl::InvalidArgumentError("Encoder is null");
    }
    // Explicit pointer alignment check natively trapping architectural failures
    CHECK_EQ(reinterpret_cast<uintptr_t>(encoder) % alignof(void*), 0)
        << "Encoder pointer alignment constraint violated";
    
    try {
      auto result = encoder->Encode(payload);
      if (!result.ok()) {
        if (absl::IsNotFound(result.status())) {
          return absl::NotFoundError("Constraint Patch Failed");
        }
        return result.status();
      }
      return result.value();
    } catch (const std::out_of_range& e) {
      return absl::NotFoundError("AST generation failed constraint checks");
    } catch (const std::exception& e) {
      return absl::InternalError(e.what());
    }
  }
};

}  // namespace decoder
}  // namespace sim
}  // namespace mpact

#endif  // MPACT_SIM_DECODER_SAFE_ENCODE_WRAPPER_H_
