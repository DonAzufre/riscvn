//===- RISCVN.cpp ----------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ABIInfoImpl.h"
#include "TargetInfo.h"

using namespace clang;
using namespace clang::CodeGen;

//===----------------------------------------------------------------------===//
// RISC-V ABI Implementation
//===----------------------------------------------------------------------===//

namespace {
class RISCVNABIInfo : public DefaultABIInfo {

public:
  RISCVNABIInfo(CodeGenTypes &CGT) : DefaultABIInfo(CGT) {}

  // DefaultABIInfo's classifyReturnType and classifyArgumentType are
  // non-virtual, but computeInfo is virtual, so we overload it.
  void computeInfo(CGFunctionInfo &FI) const override;

  ABIArgInfo classifyReturnType(QualType Ty) const;
};
} // end anonymous namespace

void RISCVNABIInfo::computeInfo(CGFunctionInfo &FI) const {
  FI.getReturnInfo() = classifyReturnType(FI.getReturnType());
  for (auto &Arg : FI.arguments())
    Arg.info = classifyArgumentType(Arg.type);
}

ABIArgInfo RISCVNABIInfo::classifyReturnType(QualType Ty) const {
  if (Ty->isAnyComplexType()) {
    return ABIArgInfo::getDirect();
  }
  else {
    return DefaultABIInfo::classifyReturnType(Ty);
  }
}

namespace {
class RISCVNTargetCodeGenInfo : public TargetCodeGenInfo {
public:
  RISCVNTargetCodeGenInfo(CodeGen::CodeGenTypes &CGT)
      : TargetCodeGenInfo(std::make_unique<RISCVNABIInfo>(CGT)) {}

  void setTargetAttributes(const Decl *D, llvm::GlobalValue *GV,
                           CodeGen::CodeGenModule &CGM) const override {}
};
} // namespace

std::unique_ptr<TargetCodeGenInfo>
CodeGen::createRISCVNTargetCodeGenInfo(CodeGenModule &CGM) {
  return std::make_unique<RISCVNTargetCodeGenInfo>(CGM.getTypes());
}
