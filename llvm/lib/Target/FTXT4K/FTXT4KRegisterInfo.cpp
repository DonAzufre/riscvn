//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KRegisterInfo.h"
#include "MCTargetDesc/FTXT4KMCTargetDesc.h"
#include "FTXT4KSubtarget.h"
#include "FTXT4K.h"

#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "FTXT4KGenRegisterInfo.inc"

using namespace FTXT4K;

constexpr int FTXT4K_STACK_BASE_ADDR = 0x100000;

FTXT4KRegisterInfo::FTXT4KRegisterInfo(): FTXT4KGenRegisterInfo(FTXT4K::R63) {
}


const MCPhysReg *FTXT4KRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
    return CSR_FTXT4K_SaveList;
}

BitVector FTXT4KRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
    BitVector Reserved(getNumRegs());
    return Reserved;
}

bool FTXT4KRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj, unsigned FIOperandNum,
                                             RegScavenger *RS) const {
    MachineInstr &Instr = *MI;
    MachineFunction &MF = *Instr.getMF();
    MachineFrameInfo &MFI = MF.getFrameInfo();

    int FrameIndex = Instr.getOperand(FIOperandNum).getIndex();
    int offset = MFI.getObjectOffset(FrameIndex);
    int StackSize = MFI.getStackSize();

    switch (Instr.getOpcode()) {
        default:
            report_fatal_error("ftxt4k: unknown inst for eliminate frame index");
        case FTXT4K::SADDI: {
            const auto addr = FTXT4K_STACK_BASE_ADDR + offset;
            BuildMI(*MI->getParent(), MI, MI->getDebugLoc(), MF.getSubtarget().getInstrInfo()->get(FTXT4K::SMOVI),
                    Instr.getOperand(0).getReg()).addImm(addr);
            MI->eraseFromParent();
        }
        break;
    }

    // report_fatal_error("ftxt4k: eliminate frame index not supported");
    return false;
}

Register FTXT4KRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
    // report_fatal_error("ftxt4k: get frame register not supported");
    return FTXT4K::AR15;
}
