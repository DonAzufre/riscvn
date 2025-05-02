#include "RISCVNRegisterInfo.h"

#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVN.h"
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
const uint32_t *
RISCVNRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                         CallingConv::ID id) const {
  return CSR_RISCVN_CALLSAVE_RegMask;
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
  Register TPReg = RISCVN::X4;

  if (!isSimm12(Offset + StackSize))
    BuildMI(*II->getParent(), II, II->getDebugLoc(),
            MF.getSubtarget().getInstrInfo()->get(RISCVN::PseudoLI), TPReg)
        .addImm(Offset + StackSize);
  switch (Instr.getOpcode()) {
  default:
    report_fatal_error("riscvn: unknown instr for eliminate frame index");
  case RISCVN::ADDI:
    if (isSimm12(Offset + StackSize)) {
      Instr.getOperand(FIOperandNum).ChangeToRegister(SPReg, false);
      Instr.getOperand(FIOperandNum + 1).setImm(Offset + StackSize);
    } else {
      BuildMI(*II->getParent(), II, II->getDebugLoc(),
              MF.getSubtarget().getInstrInfo()->get(RISCVN::ADD),
              Instr.getOperand(0).getReg())
          .addReg(SPReg)
          .addReg(TPReg);
    }
    break;
  case RISCVN::SW:
    if (isSimm12(Offset + StackSize)) {
      Instr.getOperand(FIOperandNum).ChangeToRegister(SPReg, false);
      Instr.getOperand(FIOperandNum - 1).setImm(Offset + StackSize);
    } else {
      BuildMI(*II->getParent(), II, II->getDebugLoc(),
              MF.getSubtarget().getInstrInfo()->get(RISCVN::ADD), TPReg)
          .addReg(SPReg)
          .addReg(TPReg);
      BuildMI(*II->getParent(), II, II->getDebugLoc(),
              MF.getSubtarget().getInstrInfo()->get(RISCVN::SW))
          .addReg(Instr.getOperand(0).getReg())
          .addImm(0)
          .addReg(TPReg);
    }
    break;
  case RISCVN::LW:
    if (isSimm12(Offset + StackSize)) {
      Instr.getOperand(FIOperandNum).ChangeToRegister(SPReg, false);
      Instr.getOperand(FIOperandNum - 1).setImm(Offset + StackSize);
    } else {
      BuildMI(*II->getParent(), II, II->getDebugLoc(),
              MF.getSubtarget().getInstrInfo()->get(RISCVN::ADD), TPReg)
          .addReg(SPReg)
          .addReg(TPReg);
      BuildMI(*II->getParent(), II, II->getDebugLoc(),
              MF.getSubtarget().getInstrInfo()->get(RISCVN::LW), Instr.getOperand(0).getReg())
          .addImm(0)
          .addReg(TPReg);
    }
    break;
  }

  if (!isSimm12(Offset + StackSize))
    II->eraseFromParent();

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