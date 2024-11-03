#include "RISCVNRegisterInfo.h"
#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVNSubtarget.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "RISCVNGenRegisterInfo.inc"

using namespace RISCVN;

RISCVNRegisterInfo::RISCVNRegisterInfo() : RISCVNGenRegisterInfo(RISCVN::X1) {}

const MCPhysReg *
RISCVNRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const MCPhysReg CalleeSavedRegs[] = {RISCVN::X2, 0};
  return CalleeSavedRegs;
}

BitVector RISCVNRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(RISCVN::X0);
  return Reserved;
}

bool RISCVNRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                             int SPAdj, unsigned FIOperandNum,
                                             RegScavenger *RS) const {
  return false;
}

Register RISCVNRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return RISCVN::X2;
}