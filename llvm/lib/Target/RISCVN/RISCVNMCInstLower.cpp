#include "RISCVNMCInstLower.h"

#include "MCTargetDesc/RISCVNBaseInfo.h"
#include "MCTargetDesc/RISCVNMCExpr.h"

#include "llvm/CodeGen/AsmPrinter.h"

using namespace llvm;

void RISCVNMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (const MachineOperand &MO : MI->operands()) {
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
    case MachineOperand::MO_GlobalAddress:
      MCOp = LowerSymbolOperand(MO, MO.getMCSymbol());
      break;
    }

    OutMI.addOperand(MCOp);
  }
}
MCOperand RISCVNMCInstLower::LowerSymbolOperand(const MachineOperand &MO,
                                                MCSymbol *Sym) const {
  RISCVNMCExpr::VariantKind Kind;

  switch (MO.getTargetFlags()) {
  default:
    llvm_unreachable("Unknown target flag on GV operand");
  case RISCVNII::MO_None:
    Kind = RISCVNMCExpr::VK_RISCVN_None;
    break;
  case RISCVNII::MO_HI:
    Kind = RISCVNMCExpr::VK_RISCVN_HI;
    break;
  case RISCVNII::MO_LO:
    Kind = RISCVNMCExpr::VK_RISCVN_LO;
    break;
  }

  const MCExpr *ME =
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);

  if (Kind != RISCVNMCExpr::VK_RISCVN_None)
    ME = RISCVNMCExpr::create(ME, Kind, Ctx);

  return MCOperand::createExpr(ME);
}