//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KAsmPrinter.h"
#include "MCTargetDesc/FTXT4KMCTargetDesc.h"
#include "TargetInfo/FTXT4KTargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "FTXT4KGenMCPseudoLowering.inc"

FTXT4KAsmPrinter::FTXT4KAsmPrinter(TargetMachine &TM,
                                   std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(OutContext, *this) {}

void FTXT4KAsmPrinter::emitInstruction(const MachineInstr *MI) {
    if (emitPseudoExpansionLowering(*OutStreamer, MI)) {
        return;
    }
    MCInst TmpInst;
    MCInstLowering.Lower(MI, TmpInst);
    EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFTXT4KAsmPrinter() {
    RegisterAsmPrinter<FTXT4KAsmPrinter> X(getTheFTXT4KTarget());
} 