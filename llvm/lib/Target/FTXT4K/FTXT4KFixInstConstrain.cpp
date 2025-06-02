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

#define FTXT4K_FIX_INST_CONSTRAIN_NAME "ftxt4k add memory fence to load/store inst pass"

namespace {
    class FTXT4KFixInstConstrain : public MachineFunctionPass {
    public:
        StringRef getPassName() const override;

        static char ID;

        FTXT4KFixInstConstrain() : MachineFunctionPass(ID) {
        }

    protected:
        bool runOnMachineFunction(MachineFunction &MF) override;
    };

    StringRef FTXT4KFixInstConstrain::getPassName() const {
        return FTXT4K_FIX_INST_CONSTRAIN_NAME;
    }

    bool FTXT4KFixInstConstrain::runOnMachineFunction(MachineFunction &MF) {
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

                if (Instr.getOpcode() == FTXT4K::SMVAGA) {
                    BuildMI(MBB, MI, Instr.getDebugLoc(), TII->get(FTXT4K::SNOP)).addImm(2);
                    Changed |= true;
                }

                if (Instr.getOpcode() == FTXT4K::SBR || Instr.getOpcode() == FTXT4K::CONDSBR) {
                    BuildMI(MBB, MI, Instr.getDebugLoc(), TII->get(FTXT4K::SNOP)).addImm(6);
                    Changed |= true;
                }
            }
        }

        return Changed;
    }

    char FTXT4KFixInstConstrain::ID = 0;
}

INITIALIZE_PASS(FTXT4KFixInstConstrain, "ftxt4k-add-mfence", FTXT4K_FIX_INST_CONSTRAIN_NAME, false, false);

namespace llvm {
    FunctionPass *createFTXT4KFixInstConstrainPass() {
        return new FTXT4KFixInstConstrain();
    }
}
