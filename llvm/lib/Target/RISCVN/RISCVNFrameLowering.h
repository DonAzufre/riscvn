#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNFRAMELOWERING_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class RISCVNSubtarget;

class RISCVNFrameLowering : public TargetFrameLowering {
  const RISCVNSubtarget &STI;

public:
  explicit RISCVNFrameLowering(const RISCVNSubtarget &STI)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4), 0,
                            Align(4)),
        STI(STI) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  StackOffset getFrameIndexReference(const MachineFunction &MF, int FI,
                                     Register &FrameReg) const override;
  bool
  restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator MI,
                              MutableArrayRef<CalleeSavedInfo> CSI,
                              const TargetRegisterInfo *TRI) const override;

  bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 ArrayRef<CalleeSavedInfo> CSI,
                                 const TargetRegisterInfo *TRI) const override;

  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;
  void processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                           RegScavenger *RS) const override;
};
} // namespace llvm

#endif