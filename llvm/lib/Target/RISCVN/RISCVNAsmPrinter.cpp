#include "RISCVNAsmPrinter.h"
#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "TargetInfo/RISCVNTargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"
#include "MCTargetDesc/RISCVNBaseInfo.h"
#include "MCTargetDesc/RISCVNMCExpr.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "RISCVNGenMCPseudoLowering.inc"

RISCVNAsmPrinter::RISCVNAsmPrinter(TargetMachine &TM,
                                   std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer)) {}
void RISCVNAsmPrinter::emitBasicBlockStart(const MachineBasicBlock &MBB) {
  OutStreamer->emitLabel(MBB.getSymbol());
}

void RISCVNAsmPrinter::emitInstruction(const MachineInstr *MI) {
  if (emitPseudoExpansionLowering(*OutStreamer, MI)) {
    return;
  }
  MCInst TmpInst;
  Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCVNAsmPrinter() {
  RegisterAsmPrinter<RISCVNAsmPrinter> X(getTheRISCVNTarget());
}
void RISCVNAsmPrinter::Lower(const MachineInstr *MI, MCInst &OutMI) const {
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
      MCOp = LowerSymbolOperand(MO, getSymbolPreferLocal(*MO.getGlobal()));
      break;
    case MachineOperand::MO_FrameIndex:
      // MI->dump();
      // report_fatal_error("riscvn: unimplemented asm printer lower frameindex");
      MCOp = MCOperand::createReg(RISCVN::X2);
      break;
    case MachineOperand::MO_MachineBasicBlock:
      MCOp = LowerSymbolOperand(MO, MO.getMBB()->getSymbol());
      break;
    }

    OutMI.addOperand(MCOp);
  }
}
MCOperand RISCVNAsmPrinter::LowerSymbolOperand(const MachineOperand &MO,
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
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, OutContext);

  if (Kind != RISCVNMCExpr::VK_RISCVN_None)
    ME = RISCVNMCExpr::create(ME, Kind, OutContext);

  return MCOperand::createExpr(ME);
}
