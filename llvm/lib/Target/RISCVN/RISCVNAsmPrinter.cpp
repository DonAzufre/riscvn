#include "RISCVNAsmPrinter.h"
#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "TargetInfo/RISCVNTargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "RISCVNGenMCPseudoLowering.inc"

RISCVNAsmPrinter::RISCVNAsmPrinter(TargetMachine &TM,
                                   std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(OutContext, *this) {}

void RISCVNAsmPrinter::emitInstruction(const MachineInstr *MI) {
  if (emitPseudoExpansionLowering(*OutStreamer, MI)) {
    return;
  }
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCVNAsmPrinter() {
  RegisterAsmPrinter<RISCVNAsmPrinter> X(getTheRISCVNTarget());
} 