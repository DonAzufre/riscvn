#include "RISCVNFrameLowering.h"

#include "RISCVNSubtarget.h"

#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVNRegisterInfo.h"
#include "RISCVNSubtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

void RISCVNFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  const auto &RISCVNRI = *MF.getSubtarget<RISCVNSubtarget>().getRegisterInfo();

  auto MBBI = MBB.begin();

  MachineFrameInfo &MFI = MF.getFrameInfo();
  const auto CSI = MFI.getCalleeSavedInfo();

  // DebugLoc DL = MBBI->getDebugLoc();
  DebugLoc DL;
  if (MBBI != MBB.end())
    DL = MBBI->getDebugLoc();

  auto FPReg = RISCVN::X8;
  auto SPReg = RISCVN::X2;
  auto RAReg = RISCVN::X1;

  uint64_t StackSize = MFI.getStackSize();

  while (MBBI != MBB.end() && MBBI->getFlag(MachineInstr::FrameSetup)) {
    ++MBBI;
  }

  if (StackSize == 0 && !MFI.adjustsStack())
    return;

  StackSize += 8; // for ra and fp

  // 分配栈空间
  BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADDI), SPReg)
      .addReg(SPReg)
      .addImm(-StackSize)
      .setMIFlag(MachineInstr::FrameSetup);
  // 保存ra
  BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::SW))
      .addReg(RAReg)
      .addImm(StackSize - 4)
      .addReg(SPReg);
  // 保存fp
  BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::SW))
      .addReg(FPReg)
      .addImm(StackSize - 8)
      .addReg(SPReg);
  // 构建新fp
  BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADDI), FPReg)
      .addReg(SPReg)
      .addImm(StackSize);
}

StackOffset
RISCVNFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                            Register &FrameReg) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *RI = MF.getSubtarget().getRegisterInfo();

  FrameReg = RISCVN::X2;

  return StackOffset::getFixed(MFI.getObjectOffset(FI));
}
bool RISCVNFrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    MutableArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {

  assert(!CSI.empty());

  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
  DebugLoc DL;
  if (MI != MBB.end() && !MI->isDebugInstr())
    DL = MI->getDebugLoc();

  for (const CalleeSavedInfo &Info : CSI) {
    Register Reg = Info.getReg();
    int FI = Info.getFrameIdx();

    BuildMI(MBB, MI, DL, TII.get(RISCVN::LW), Reg)
        .addImm(0)
        .addFrameIndex(FI);
  }
  return true;
}

bool RISCVNFrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {

  assert(!CSI.empty());

  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
  DebugLoc DL;
  if (MI != MBB.end() && !MI->isDebugInstr())
    DL = MI->getDebugLoc();

  for (const CalleeSavedInfo &Info : CSI) {
    Register Reg = Info.getReg();
    int FI = Info.getFrameIdx();

    BuildMI(MBB, MI, DL, TII.get(RISCVN::SW))
        .addReg(Reg, RegState::Kill)
        .addImm(0)
        .addFrameIndex(FI);
  }

  return true;

  // report_fatal_error("riscvn: spill regs not implemented");
}

void RISCVNFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {

  const auto &RISCVNRI = *MF.getSubtarget<RISCVNSubtarget>().getRegisterInfo();

  auto MBBI = MBB.getLastNonDebugInstr();

  MachineFrameInfo &MFI = MF.getFrameInfo();
  const auto CSI = MFI.getCalleeSavedInfo();

  DebugLoc DL = MBBI->getDebugLoc();

  auto FPReg = RISCVN::X8;
  auto SPReg = RISCVN::X2;
  auto RAReg = RISCVN::X1;

  uint64_t StackSize = MFI.getStackSize();

  if (StackSize == 0 && !MFI.adjustsStack())
    return;

  StackSize += 8; // for ra and fp

  BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::LW), RAReg)
      .addImm(StackSize - 4)
      .addReg(SPReg);
  BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::LW), FPReg)
      .addImm(StackSize - 8)
      .addReg(SPReg);
  BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADDI), SPReg)
      .addReg(SPReg)
      .addImm(StackSize);
}

bool RISCVNFrameLowering::hasFP(const MachineFunction &MF) const {
  return true;
}
void RISCVNFrameLowering::processFunctionBeforeFrameFinalized(
    MachineFunction &MF, RegScavenger *RS) const {
  auto &MFI = MF.getFrameInfo();

  MFI.estimateStackSize(MF);
}