#include "RISCVNSubtarget.h"

using namespace llvm;

#define DEBUG_TYPE "RISCVN-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "RISCVNGenSubtargetInfo.inc"

void RISCVNSubtarget::anchor() {}

RISCVNSubtarget &
RISCVNSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                 const TargetMachine &TM) {
  if (CPU.empty()) {
    CPU = "riscvn";
  }
  ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);

  return *this;
}

RISCVNSubtarget::RISCVNSubtarget(const Triple &TT, const std::string &CPU,
                                 const std::string &FS, const TargetMachine &TM)
    : RISCVNGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      FrameLowering(initializeSubtargetDependencies(CPU, FS, TM)),
      InstrInfo(*this), TLInfo(TM, *this), RegInfo() {}