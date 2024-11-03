#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNMCINSTLOWER_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNMCINSTLOWER_H

#include "llvm/Support/Compiler.h"

namespace llvm {
class AsmPrinter;
class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;

class LLVM_LIBRARY_VISIBILITY RISCVNMCInstLower {
  MCContext &Ctx;
  AsmPrinter &Printer;

public:
  RISCVNMCInstLower(MCContext &Ctx, AsmPrinter &Printer)
      : Ctx(Ctx), Printer(Printer) {}

  void Lower(const MachineInstr *MI, MCInst &OutMI) const;
};
} // namespace llvm

#endif