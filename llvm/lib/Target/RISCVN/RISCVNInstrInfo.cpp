#include "RISCVNInstrInfo.h"

#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVNSubtarget.h"

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
      .addImm(0)
      .addFrameIndex(FrameIndex);
}

void RISCVNInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MBBI,
                                  const DebugLoc &DL, MCRegister DestReg,
                                  MCRegister SrcReg, bool KillSrc) const {
  // todo: 处理寄存器类
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();
  BuildMI(MBB, MBBI, DL, get(RISCVN::ADDI), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc))
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
      .addImm(0)
      .addReg(FrameIndex);
}
RISCVNInstrInfo::RISCVNInstrInfo(RISCVNSubtarget &STI)
    : RISCVNGenInstrInfo(RISCVN::ADJCALLSTACKDOWN, RISCVN::ADJCALLSTACKUP, 0,
                         RISCVN::PseudoRET),
      STI(STI) {}