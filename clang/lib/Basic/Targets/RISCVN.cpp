//===--- RISCVN.cpp - Implement RISCVN target feature support
//--------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements RISC-V TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "RISCVN.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/TargetParser/RISCVTargetParser.h"
#include <optional>

namespace clang {
namespace targets {
RISCVNTargetInfo::RISCVNTargetInfo(const llvm::Triple &Triple,
                                   const TargetOptions &)
    : TargetInfo(Triple) {
  IntPtrType = SignedInt;
  PtrDiffType = SignedInt;
  SizeType = UnsignedInt;
  resetDataLayout("e-m:e-p:32:32-i32:32-n32-S32");
}

void RISCVNTargetInfo::getTargetDefines(const LangOptions &Opts,
                                        MacroBuilder &Builder) const {}

bool RISCVNTargetInfo::validateAsmConstraint(
    const char *&Name, TargetInfo::ConstraintInfo &info) const {
  return false;
}

} // namespace targets
} // namespace clang