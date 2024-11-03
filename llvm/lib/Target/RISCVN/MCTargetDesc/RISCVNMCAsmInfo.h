#ifndef LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNMCASMINFO_H
#define LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class RISCVNMCAsmInfo : public MCAsmInfoELF {
public:
  explicit RISCVNMCAsmInfo(const Triple &TargetTriple);
};
} // namespace llvm

#endif