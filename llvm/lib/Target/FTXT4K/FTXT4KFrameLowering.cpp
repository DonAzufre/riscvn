//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KFrameLowering.h"

using namespace llvm;

void FTXT4KFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}

void FTXT4KFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}

bool FTXT4KFrameLowering::hasFP(const MachineFunction &MF) const {
    return false;
}