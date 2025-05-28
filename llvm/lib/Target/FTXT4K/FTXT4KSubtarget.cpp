//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KSubtarget.h"


using namespace llvm;

#define DEBUG_TYPE "FTXT4K-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "FTXT4KGenSubtargetInfo.inc"

void FTXT4KSubtarget::anchor() {}

FTXT4KSubtarget &
FTXT4KSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                 const TargetMachine &TM) {
    if (CPU.empty()) {
        CPU = "ftxt4k";
    }
    ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);

    return *this;
}

FTXT4KSubtarget::FTXT4KSubtarget(const Triple &TT, const std::string &CPU,
                                 const std::string &FS, const TargetMachine &TM)
    : FTXT4KGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      FrameLowering(initializeSubtargetDependencies(CPU, FS, TM)),
      InstrInfo(*this), TLInfo(TM, *this), RegInfo() {}