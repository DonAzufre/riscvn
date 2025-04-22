#include "RISCVNInstrInfo.h"
#include "MCTargetDesc/RISCVNMCTargetDesc.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "RISCVNGenInstrInfo.inc"

void RISCVNInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                           MachineBasicBlock::iterator MI,
                                           Register DestReg, int FrameIndex,
                                           const TargetRegisterClass *RC,
                                           const TargetRegisterInfo *TRI,
                                           Register VReg) const {
  DebugLoc DL;
  MachineFunction &MF = *MBB.getParent();
  if (MI != MBB.end())
    DL = MI->getDebugLoc();

  // reg class 相关处理
  BuildMI(MBB, MI, DL, get(RISCVN::LW), DestReg)
      .addFrameIndex(FrameIndex)
      .addImm(0);
}
void RISCVNInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register SrcReg,
    bool isKill, int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg) const {
  DebugLoc DL;
  MachineFunction &MF = *MBB.getParent();
  auto KillFlag = getKillRegState(isKill);
  if (MI != MBB.end())
    DL = MI->getDebugLoc();

  // reg class相关处理
  BuildMI(MBB, MI, DL, get(RISCVN::SW))
      .addReg(SrcReg, KillFlag)
      .addReg(FrameIndex)
      .addImm(0);
}
RISCVNInstrInfo::RISCVNInstrInfo(RISCVNSubtarget &STI)
    : RISCVNGenInstrInfo(), STI(STI) {}