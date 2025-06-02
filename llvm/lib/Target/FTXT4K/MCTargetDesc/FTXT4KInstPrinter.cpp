//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KInstPrinter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCExpr.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "FTXT4KGenAsmWriter.inc"

void FTXT4KInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                  StringRef Annot, const MCSubtargetInfo &STI,
                                  raw_ostream &O) {
    if (!printAliasInstr(MI, Address, O)) {
        printInstruction(MI, Address, O);
    }
}

void FTXT4KInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O, const char *Modifier) {
    assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
    const MCOperand &Op = MI->getOperand(OpNo);
    if (Op.isReg()) {
        O << getRegisterName(Op.getReg());
    } else if (Op.isImm()) {
        O << formatImm((int32_t)Op.getImm());
    } else {
        assert(Op.isExpr() && "Expected an expression");
    }
}

void FTXT4KInstPrinter::printJmpTargetOperand(const MCInst *MI, uint64_t Address, unsigned OpNo, raw_ostream &O,
    const char *Modifier) {
    assert(MI->getOperand(OpNo).isExpr() && "jmptarget must be a label(Expr).");
    MI->getOperand(OpNo).getExpr()->print(O, &MAI);
}

void FTXT4KInstPrinter::printCondExecuteCtlOperand(const MCInst *MI, uint64_t Address, unsigned OpNo, raw_ostream &O,
    const char *Modifier) {
    assert(MI->getOperand(OpNo).isImm() && "cond execute control must be an imm");
    const auto imm = MI->getOperand(OpNo).getImm();
    if (imm != 0)
        O << "!";
}
