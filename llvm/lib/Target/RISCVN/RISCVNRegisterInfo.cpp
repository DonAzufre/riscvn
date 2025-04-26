#include "RISCVNRegisterInfo.h"
#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVNSubtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "RISCVNGenRegisterInfo.inc"

using namespace RISCVN;

RISCVNRegisterInfo::RISCVNRegisterInfo() : RISCVNGenRegisterInfo(RISCVN::X1) {}

const MCPhysReg *
RISCVNRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_RISCVN_SaveList;
}

BitVector RISCVNRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(RISCVN::X0);
  Reserved.set(RISCVN::X1); // ra
  Reserved.set(RISCVN::X2); // sp
  Reserved.set(RISCVN::X3); // gp
  Reserved.set(RISCVN::X4); // tp
  Reserved.set(RISCVN::X8); // fp
  return Reserved;
}

bool RISCVNRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                             int SPAdj, unsigned FIOperandNum,
                                             RegScavenger *RS) const {
  MachineInstr &Instr = *II;
  MachineFunction &MF = *Instr.getMF();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const RISCVNFrameLowering *TFI = static_cast<const RISCVNFrameLowering *>(
      MF.getSubtarget().getFrameLowering());

  int FrameIndex = Instr.getOperand(FIOperandNum).getIndex();

  int Offset = MFI.getObjectOffset(FrameIndex);
  int StackSize = MFI.getStackSize();

  // todo: 统一处理方式

  Register SPReg = RISCVN::X2;
  switch (Instr.getOpcode()) {
  default:
    report_fatal_error("riscvn: unknown instr for eliminate frame index");
  case RISCVN::ADDI:
    Instr.getOperand(FIOperandNum).ChangeToRegister(SPReg, false);
    Instr.getOperand(FIOperandNum + 1).setImm(Offset + StackSize);
    break;
  case RISCVN::SW:
    Instr.getOperand(FIOperandNum).ChangeToRegister(SPReg, false);
    Instr.getOperand(FIOperandNum - 1).setImm(Offset + StackSize);
    break;
  case RISCVN::LW:
    Instr.getOperand(FIOperandNum).ChangeToRegister(SPReg, false);
    Instr.getOperand(FIOperandNum - 1).setImm(Offset + StackSize);
    break;
  }

  // // 添加用于保存ra和fp的大小
  // StackSize += 8;
  // assert(Instr.getOperand(FIOperandNum + 1).getImm() == 0 &&
  //        "riscvn: frameindex not in format addi sp, 0");

  // Offset += StackSize + 4;
  //
  // auto FPReg = getFrameRegister(MF);
  //
  // Instr.getOperand(FIOperandNum).ChangeToRegister(FPReg, false);
  // Instr.getOperand(FIOperandNum + 1).setImm(-Offset);

  return false;
}

Register RISCVNRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return RISCVN::X8;
}