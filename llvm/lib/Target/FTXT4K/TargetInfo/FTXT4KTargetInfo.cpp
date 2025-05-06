//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheFTXT4KTarget() {
  static Target TheFTXT4KTarget;
  return TheFTXT4KTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFTXT4KTargetInfo() {
  RegisterTarget<Triple::ftxt4k, false> X(getTheFTXT4KTarget(), "ftxt4k", "FT_XT4000 little endian", "FTXT4K");
}