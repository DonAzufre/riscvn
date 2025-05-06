//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KFRAMELOWERING_H
#define FTXT4KFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
    class FTXT4KSubtarget;

    class FTXT4KFrameLowering : public TargetFrameLowering {
        const FTXT4KSubtarget &STI;

    public:
        explicit FTXT4KFrameLowering(const FTXT4KSubtarget &STI)
            : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4), 0,
                                  Align(4)),
              STI(STI) {}

        void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

        void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

        bool hasFP(const MachineFunction &MF) const override;
    };
} // namespace llvm

#endif //FTXT4KFRAMELOWERING_H
