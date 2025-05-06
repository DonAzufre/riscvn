//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KREGISTERINFO_H
#define FTXT4KREGISTERINFO_H

#define GET_REGINFO_HEADER
#include "FTXT4KGenRegisterInfo.inc"

namespace llvm {
    struct FTXT4KRegisterInfo : public FTXT4KGenRegisterInfo {
        FTXT4KRegisterInfo();

        const MCPhysReg * getCalleeSavedRegs(const MachineFunction *MF) const override;

        BitVector getReservedRegs(const MachineFunction &MF) const override;

        bool eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj, unsigned FIOperandNum,
            RegScavenger *RS) const override;

        Register getFrameRegister(const MachineFunction &MF) const override;
    };
}

#endif //FTXT4KREGISTERINFO_H
