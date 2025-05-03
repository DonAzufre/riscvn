#include "RISCVNFrameLowering.h"

#include "RISCVN.h"
#include "RISCVNSubtarget.h"

#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVNRegisterInfo.h"
#include "RISCVNSubtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

MachineBasicBlock::iterator RISCVNFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MI) const {
  Register SPReg = RISCVN::X2;
  Register TPReg = RISCVN::X4;
  DebugLoc DL = MI->getDebugLoc();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  // hasReservedCallFrame always true here

  int64_t Amount = MI->getOperand(0).getImm();

  if (Amount != 0) {
    // align sp here
    if (MI->getOpcode() == RISCVN::ADJCALLSTACKDOWN)
      Amount = -Amount;

    const RISCVNRegisterInfo &RI = *STI.getRegisterInfo();
    if (isSimm12(Amount))
      BuildMI(MBB, MI, DL, TII.get(RISCVN::ADDI), SPReg)
          .addReg(SPReg)
          .addImm(Amount)
          .setMIFlag(MachineInstr::NoFlags);
    else {
      BuildMI(MBB, MI, DL, TII.get(RISCVN::PseudoLI), TPReg)
          .addImm(Amount)
          .setMIFlag(MachineInstr::NoFlags);
      BuildMI(MBB, MI, DL, TII.get(RISCVN::ADD), SPReg)
          .addReg(SPReg)
          .addReg(TPReg)
          .setMIFlag(MachineInstr::NoFlags);
    }
  }

  return MBB.erase(MI);
}

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
  if (isSimm12(-StackSize)) {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADDI), SPReg)
        .addReg(SPReg)
        .addImm(-StackSize)
        .setMIFlag(MachineInstr::FrameSetup);
  } else {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::PseudoLI),
            RISCVN::X4)
        .addImm(-StackSize);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADD), SPReg)
        .addReg(SPReg)
        .addReg(RISCVN::X4)
        .setMIFlag(MachineInstr::FrameSetup);
  }
  // 保存ra
  if (isSimm12(StackSize - 4)) {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::SW))
        .addReg(RAReg)
        .addImm(StackSize - 4)
        .addReg(SPReg);
  } else {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::PseudoLI),
            RISCVN::X4)
        .addImm(StackSize - 4);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADD), RISCVN::X4)
        .addReg(SPReg)
        .addReg(RISCVN::X4);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::SW))
        .addReg(RAReg)
        .addImm(0)
        .addReg(RISCVN::X4);
  }
  // 保存fp
  if (isSimm12(StackSize - 8)) {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::SW))
        .addReg(FPReg)
        .addImm(StackSize - 8)
        .addReg(SPReg);
  } else {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::PseudoLI),
            RISCVN::X4)
        .addImm(StackSize - 8);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADD), RISCVN::X4)
        .addReg(SPReg)
        .addReg(RISCVN::X4);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::SW))
        .addReg(FPReg)
        .addImm(0)
        .addReg(RISCVN::X4);
  }
  // 构建新fp
  if (isSimm12(StackSize)) {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADDI), FPReg)
        .addReg(SPReg)
        .addImm(StackSize);
  } else {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::PseudoLI),
            RISCVN::X4)
        .addImm(StackSize);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADD), FPReg)
        .addReg(SPReg)
        .addReg(RISCVN::X4);
  }
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

    BuildMI(MBB, MI, DL, TII.get(RISCVN::LW), Reg).addImm(0).addFrameIndex(FI);
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

  if (isSimm12(StackSize - 4)) {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::LW), RAReg)
        .addImm(StackSize - 4)
        .addReg(SPReg);
  } else {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::PseudoLI),
            RISCVN::X4)
        .addImm(StackSize - 4);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADD), RISCVN::X4)
        .addReg(SPReg)
        .addReg(RISCVN::X4);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::LW), RAReg)
        .addImm(0)
        .addReg(RISCVN::X4);
  }
  if (isSimm12(StackSize - 8)) {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::LW), FPReg)
        .addImm(StackSize - 8)
        .addReg(SPReg);
  } else {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::PseudoLI),
            RISCVN::X4)
        .addImm(StackSize - 8);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADD), RISCVN::X4)
        .addReg(SPReg)
        .addReg(RISCVN::X4);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::LW), FPReg)
        .addImm(0)
        .addReg(RISCVN::X4);
  }
  if (isSimm12(StackSize)) {
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADDI), SPReg)
        .addReg(SPReg)
        .addImm(StackSize);
  } else {
    // auto TmpReg =
    // MF.getRegInfo().createVirtualRegister(&RISCVN::GPRRegClass);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::PseudoLI),
            RISCVN::X4)
        .addImm(StackSize);
    BuildMI(MBB, MBBI, DL, STI.getInstrInfo()->get(RISCVN::ADD), FPReg)
        .addReg(SPReg)
        .addReg(RISCVN::X4);
  }
}

bool RISCVNFrameLowering::hasFP(const MachineFunction &MF) const {
  return true;
}
void RISCVNFrameLowering::processFunctionBeforeFrameFinalized(
    MachineFunction &MF, RegScavenger *RS) const {
  auto &MFI = MF.getFrameInfo();

  MFI.estimateStackSize(MF);
}