//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KRegisterInfo.h"
#include "MCTargetDesc/FTXT4KMCTargetDesc.h"
#include "FTXT4KSubtarget.h"
#include "FTXT4K.h"

#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "FTXT4KGenRegisterInfo.inc"

using namespace FTXT4K;

FTXT4KRegisterInfo::FTXT4KRegisterInfo(): FTXT4KGenRegisterInfo(FTXT4K::R63) {
}


const MCPhysReg * FTXT4KRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
    return CSR_FTXT4K_SaveList;
}

BitVector FTXT4KRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
    BitVector Reserved(getNumRegs());
    return Reserved;
}

bool FTXT4KRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj, unsigned FIOperandNum,
    RegScavenger *RS) const {
    report_fatal_error("ftxt4k: eliminate frame index not supported");
}

Register FTXT4KRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
    report_fatal_error("ftxt4k: get frame register not supported");
}
