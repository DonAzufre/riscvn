//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KInstPrinter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"

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