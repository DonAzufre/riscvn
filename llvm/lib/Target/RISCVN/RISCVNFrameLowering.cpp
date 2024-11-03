#include "RISCVNFrameLowering.h"

using namespace llvm;

void RISCVNFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}

void RISCVNFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}

bool RISCVNFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}