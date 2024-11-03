#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNREGISTERINFO_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNREGISTERINFO_H

#define GET_REGINFO_HEADER
#include "RISCVNGenRegisterInfo.inc"

namespace llvm {
struct RISCVNRegisterInfo : public RISCVNGenRegisterInfo {
  RISCVNRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // namespace llvm

#endif