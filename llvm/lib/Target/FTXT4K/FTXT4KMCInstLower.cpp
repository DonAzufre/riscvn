//
// Created by DonAzufre on 25-5-5.
//
#include "FTXT4KMCInstLower.h"
#include "llvm/CodeGen/AsmPrinter.h"

using namespace llvm;

void FTXT4KMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
    OutMI.setOpcode(MI->getOpcode());

    for (const MachineOperand &MO: MI->operands()) {
        MCOperand MCOp;
        switch (MO.getType()) {
            default:
                llvm_unreachable("unknown operand type");
            case MachineOperand::MO_Register:
                if (MO.isImplicit())
                    continue;
                MCOp = MCOperand::createReg(MO.getReg());
                break;
            case MachineOperand::MO_Immediate:
                MCOp = MCOperand::createImm(MO.getImm());
                break;
            case MachineOperand::MO_MachineBasicBlock:
                const auto symbol = MO.getMBB()->getSymbol();
                MCOp = MCOperand::createExpr(MCSymbolRefExpr::create(symbol, Ctx));
                break;
        }

        OutMI.addOperand(MCOp);
    }
}
