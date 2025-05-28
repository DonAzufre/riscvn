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

constexpr char OpenUnits[] =
        "\tSMOVI\t.M1\t0x3,\tR63\n"
        "\tSMVCGC\t.L\tR63,\tCVCCR\n"
        "\tSNOP\t0x5\n"
        "\tSMFENCE\n"
        "\tSMOVI\t.M1\t0x3,\tR63\n"
        "\tSMVCGC\t.L\tR63,\tSCR\n"
        "\tSMFENCE";

constexpr char AbortSimulator[] =
        "\tSMVAIA\t.M1\t0x23039FF10,\tAR14\n"
        "\t|\tSMOVI\t.M2\t0x12345678,\tR63\n"
        "\tSNOP\t0x1\n"
        "\tSSTW\tR63,\t*+AR14[0x0]\n";

FTXT4KAsmPrinter::FTXT4KAsmPrinter(TargetMachine &TM,
                                   std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(OutContext, *this) {
}

void FTXT4KAsmPrinter::emitStartOfAsmFile(Module &module) {
    OutStreamer->emitRawText("\t.section .text.0\n");
    OutStreamer->emitRawText(OpenUnits);
    AsmPrinter::emitStartOfAsmFile(module);
}

void FTXT4KAsmPrinter::emitFunctionBodyEnd() {
    OutStreamer->emitRawText(AbortSimulator);
    AsmPrinter::emitFunctionBodyEnd();
}

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
