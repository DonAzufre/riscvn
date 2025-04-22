#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNINSTRINFO_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNINSTRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "RISCVNGenInstrInfo.inc"

namespace llvm {

class RISCVNSubtarget;

class RISCVNInstrInfo : public RISCVNGenInstrInfo {
public:
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI, Register DestReg,
                            int FrameIndex, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI,
                            Register VReg) const override;
  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MI, Register SrcReg,
                           bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI,
                           Register VReg) const override;

private:
  const RISCVNSubtarget &STI;

public:
  explicit RISCVNInstrInfo(RISCVNSubtarget &STI);
};

} // namespace llvm

#endif