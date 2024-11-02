#include "RISCVNTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheRISCVNTarget() {
  static Target TheRISCVNTarget;
  return TheRISCVNTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCVNTargetInfo() {
  RegisterTarget<Triple::riscvn, /*HasJIT=*/false> X(
      getTheRISCVNTarget(), "riscvn", "RISCVN (little endian)", "RISCVN");
}