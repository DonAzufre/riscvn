#include "RISCVNInstPrinter.h"

#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "RISCVNGenAsmWriter.inc"

void RISCVNInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                  StringRef Annot, const MCSubtargetInfo &STI,
                                  raw_ostream &O) {
  if (!printAliasInstr(MI, Address, O)) {
    printInstruction(MI, Address, O);
  }
}

void RISCVNInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg(), RISCVN::ABIRegAltName);
  } else if (Op.isImm()) {
    O << formatImm((int32_t)Op.getImm());
  } else {
    assert(Op.isExpr() && "Expected an expression");
    Op.getExpr()->print(O, &MAI);
  }
}
void RISCVNInstPrinter::printJmpTargetOperand(const MCInst *MI,
                                              uint64_t Address, unsigned OpNo,
                                              raw_ostream &O,
                                              const char *Modifier) {
  assert(MI->getOperand(OpNo).isExpr() && "jmptarget must be a label(Expr).");
  MI->getOperand(OpNo).getExpr()->print(O, &MAI);
}
void RISCVNInstPrinter::printCallSymbolOperand(const MCInst *MI,
                                               uint64_t Address, unsigned OpNo,
                                               raw_ostream &O,
                                               const char *Modifier) {
  const auto &OP = MI->getOperand(OpNo);
  assert(OP.isExpr() && "call symbol must be a label(Expr).");
  OP.getExpr()->print(O, &MAI);
}

const char *RISCVNInstPrinter::getRegisterName(MCRegister Reg) {
  return getRegisterName(Reg, RISCVN::NoRegAltName);
}