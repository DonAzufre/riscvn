//
// Created by myrica on 4/30/25.
//

#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVN.h"
#include "RISCVNInstrInfo.h"
#include "RISCVNTargetMachine.h"

#include "llvm/CodeGen/LivePhysRegs.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/MCContext.h"

using namespace llvm;

#define RISCVN_EXPAND_PSEUDO_NAME "RISCVN pseudo instruction expansion pass"

namespace {
class RISCVNExpandPseudo : public MachineFunctionPass {
public:
  StringRef getPassName() const override { return RISCVN_EXPAND_PSEUDO_NAME; };

protected:
  bool runOnMachineFunction(MachineFunction &MF) override;

public:
  static char ID;
  RISCVNExpandPseudo() : MachineFunctionPass(ID) {}
};
bool RISCVNExpandPseudo::runOnMachineFunction(MachineFunction &MF) {
  return false;
  const auto &TII = MF.getSubtarget().getInstrInfo();
  bool Changed = false;

  for (auto &MBB : MF) {
    for (auto MII = MBB.begin(); MII != MBB.end();) {
      MachineInstr &MI = *MII++;

      switch (MI.getOpcode()) {
      default:
        break;
      case RISCVN::PseudoLI:
        DebugLoc DL = MI.getDebugLoc();
        Register DstReg = MI.getOperand(0).getReg();
        Register TmpReg =
            MF.getRegInfo().createVirtualRegister(&RISCVN::GPRRegClass);
        int64_t imm = MI.getOperand(1).getImm();
        assert(isInt<32>(imm));
        // assert(imm >= 0);

        // todo: 验证正确性
        auto HI = (imm + 0x800) >> 12;
        auto LO = SignExtend64<12>(imm);

        BuildMI(MBB, MI, DL, TII->get(RISCVN::LUI), TmpReg).addImm(HI);
        BuildMI(MBB, MI, DL, TII->get(RISCVN::ADDI), DstReg)
            .addReg(TmpReg)
            .addImm(LO);

        MI.eraseFromParent();
        Changed = true;
        break;
      }
    }
  }

  return Changed;
}

char RISCVNExpandPseudo::ID = 0;

} // namespace

INITIALIZE_PASS(RISCVNExpandPseudo, "riscvn-expand-pseudo",
                RISCVN_EXPAND_PSEUDO_NAME, false, false)

namespace llvm {

FunctionPass *createRISCVNExpandPseudoPass() {
  return new RISCVNExpandPseudo();
}
} // namespace llvm