#include "RISCVNInstrInfo.h"
#include "MCTargetDesc/RISCVNMCTargetDesc.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "RISCVNGenInstrInfo.inc"

RISCVNInstrInfo::RISCVNInstrInfo(RISCVNSubtarget &STI)
    : RISCVNGenInstrInfo(), STI(STI) {}