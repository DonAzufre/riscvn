//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KInstrInfo.h"
#include "MCTargetDesc/FTXT4KMCTargetDesc.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "FTXT4KGenInstrInfo.inc"

FTXT4KInstrInfo::FTXT4KInstrInfo(FTXT4KSubtarget &STI) : FTXT4KGenInstrInfo(), STI(STI) {}
