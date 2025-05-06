//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KASMPRINTER_H
#define FTXT4KASMPRINTER_H

#include "FTXT4KMCInstLower.h"
#include "llvm/CodeGen/AsmPrinter.h"

namespace llvm {

    class LLVM_LIBRARY_VISIBILITY FTXT4KAsmPrinter : public AsmPrinter {
        FTXT4KMCInstLower MCInstLowering;

    public:
        explicit FTXT4KAsmPrinter(TargetMachine &TM,
                                  std::unique_ptr<MCStreamer> Streamer);

        StringRef getPassName() const override { return "FTXT4K Assembly Printer"; }

        void emitInstruction(const MachineInstr *MI) override;

    private:
        bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                         const MachineInstr *MI);
    };
} // end namespace llvm

#endif //FTXT4KASMPRINTER_H
