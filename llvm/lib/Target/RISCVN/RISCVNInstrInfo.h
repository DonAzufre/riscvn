#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNINSTRINFO_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNINSTRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "RISCVNGenInstrInfo.inc"

namespace llvm {

class RISCVNSubtarget;

class RISCVNInstrInfo : public RISCVNGenInstrInfo {
  const RISCVNSubtarget &STI;

public:
  explicit RISCVNInstrInfo(RISCVNSubtarget &STI);
};

} // namespace llvm

#endif