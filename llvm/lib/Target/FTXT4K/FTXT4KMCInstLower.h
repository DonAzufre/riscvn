//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KMCINSTLOWER_H
#define FTXT4KMCINSTLOWER_H

#include "llvm/Support/Compiler.h"

namespace llvm {
    class AsmPrinter;
    class MCContext;
    class MCInst;
    class MCOperand;
    class MachineInstr;
    class MachineOperand;

    class LLVM_LIBRARY_VISIBILITY FTXT4KMCInstLower {
        MCContext &Ctx;
        AsmPrinter &Printer;

    public:
        FTXT4KMCInstLower(MCContext &Ctx, AsmPrinter &Printer)
            : Ctx(Ctx), Printer(Printer) {}

        void Lower(const MachineInstr *MI, MCInst &OutMI) const;
    };
} // namespace llvm

#endif //FTXT4KMCINSTLOWER_H
