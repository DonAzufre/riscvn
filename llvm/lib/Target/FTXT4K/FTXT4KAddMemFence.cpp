//
// Created by DonAzufre on 25-5-29.
//

#include "FTXT4K.h"
#include "MCTargetDesc/FTXT4KMCTargetDesc.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

#define FTXT4K_ADD_MEM_FENCE_NAME "ftxt4k add memory fence to load/store inst pass"

namespace {
    class FTXT4KAddMemFence : public MachineFunctionPass {
    public:
        StringRef getPassName() const override;

        static char ID;

        FTXT4KAddMemFence() : MachineFunctionPass(ID) {
        }

    protected:
        bool runOnMachineFunction(MachineFunction &MF) override;
    };

    StringRef FTXT4KAddMemFence::getPassName() const {
        return FTXT4K_ADD_MEM_FENCE_NAME;
    }

    bool FTXT4KAddMemFence::runOnMachineFunction(MachineFunction &MF) {
        const auto &TII = MF.getSubtarget().getInstrInfo();
        bool Changed = false;

        for (auto &MBB: MF) {
            for (auto MI = MBB.begin(); MI != MBB.end(); /* no increment */) {
                MachineInstr &Instr = *MI;
                ++MI;

                if (Instr.mayLoadOrStore()) {
                    BuildMI(MBB, MI, Instr.getDebugLoc(), TII->get(FTXT4K::SMFENCE));
                    Changed |= true;
                }
            }
        }

        return Changed;
    }

    char FTXT4KAddMemFence::ID = 0;
}

INITIALIZE_PASS(FTXT4KAddMemFence, "ftxt4k-add-mfence", FTXT4K_ADD_MEM_FENCE_NAME, false, false);

namespace llvm {
    FunctionPass *createFTXT4KAddMemFencePass() {
        return new FTXT4KAddMemFence();
    }
}
