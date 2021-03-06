/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_RUNTIME_ARCH_ARM_INSTRUCTION_SET_FEATURES_ARM_H_
#define ART_RUNTIME_ARCH_ARM_INSTRUCTION_SET_FEATURES_ARM_H_

#include <museum/6.0.1/art/runtime/arch/instruction_set_features.h>

namespace facebook { namespace museum { namespace MUSEUM_VERSION { namespace art {

// Instruction set features relevant to the ARM architecture.
class ArmInstructionSetFeatures FINAL : public InstructionSetFeatures {
 public:
  // Process a CPU variant string like "krait" or "cortex-a15" and create InstructionSetFeatures.
  static const ArmInstructionSetFeatures* FromVariant(const std::string& variant,
                                                      std::string* error_msg);

  // Parse a bitmap and create an InstructionSetFeatures.
  static const ArmInstructionSetFeatures* FromBitmap(uint32_t bitmap);

  // Turn C pre-processor #defines into the equivalent instruction set features.
  static const ArmInstructionSetFeatures* FromCppDefines();

  // Process /proc/cpuinfo and use kRuntimeISA to produce InstructionSetFeatures.
  static const ArmInstructionSetFeatures* FromCpuInfo();

  // Process the auxiliary vector AT_HWCAP entry and use kRuntimeISA to produce
  // InstructionSetFeatures.
  static const ArmInstructionSetFeatures* FromHwcap();

  // Use assembly tests of the current runtime (ie kRuntimeISA) to determine the
  // InstructionSetFeatures. This works around kernel bugs in AT_HWCAP and /proc/cpuinfo.
  static const ArmInstructionSetFeatures* FromAssembly();

  bool Equals(const InstructionSetFeatures* other) const OVERRIDE;

  InstructionSet GetInstructionSet() const OVERRIDE {
    return kArm;
  }

  uint32_t AsBitmap() const OVERRIDE;

  // Return a string of the form "div,lpae" or "none".
  std::string GetFeatureString() const OVERRIDE;

  // Is the divide instruction feature enabled?
  bool HasDivideInstruction() const {
      return has_div_;
  }

  // Are the ldrd and strd instructions atomic? This is commonly true when the Large Physical
  // Address Extension (LPAE) is present.
  bool HasAtomicLdrdAndStrd() const {
    return has_atomic_ldrd_strd_;
  }

  virtual ~ArmInstructionSetFeatures() {}

 protected:
  // Parse a vector of the form "div", "lpae" adding these to a new ArmInstructionSetFeatures.
  const InstructionSetFeatures*
      AddFeaturesFromSplitString(const bool smp, const std::vector<std::string>& features,
                                 std::string* error_msg) const OVERRIDE;

 private:
  ArmInstructionSetFeatures(bool smp, bool has_div, bool has_atomic_ldrd_strd)
      : InstructionSetFeatures(smp),
        has_div_(has_div), has_atomic_ldrd_strd_(has_atomic_ldrd_strd) {
  }

  // Bitmap positions for encoding features as a bitmap.
  enum {
    kSmpBitfield = 1,
    kDivBitfield = 2,
    kAtomicLdrdStrdBitfield = 4,
  };

  const bool has_div_;
  const bool has_atomic_ldrd_strd_;

  DISALLOW_COPY_AND_ASSIGN(ArmInstructionSetFeatures);
};

} } } } // namespace facebook::museum::MUSEUM_VERSION::art

#endif  // ART_RUNTIME_ARCH_ARM_INSTRUCTION_SET_FEATURES_ARM_H_
