#ifndef LLVM_LIB_TARGET_RISCVN_ARMASMPRINTER_H
#define LLVM_LIB_TARGET_RISCVN_ARMASMPRINTER_H

#include "llvm/CodeGen/AsmPrinter.h"

namespace llvm {

class LLVM_LIBRARY_VISIBILITY RISCVNAsmPrinter : public AsmPrinter {

public:
  explicit RISCVNAsmPrinter(TargetMachine &TM,
                            std::unique_ptr<MCStreamer> Streamer);
  void emitBasicBlockStart(const MachineBasicBlock &MBB) override;

  StringRef getPassName() const override { return "RISCVN Assembly Printer"; }

  void emitInstruction(const MachineInstr *MI) override;

private:
  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

  MCOperand LowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const;
};
} // end namespace llvm

#endif