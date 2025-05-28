//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4K_H
#define FTXT4K_H

#include "llvm/PassRegistry.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
    class FTXT4KTargetMachine;
    class FunctionPass;

    FunctionPass *createFTXT4KISelDag(FTXT4KTargetMachine &TM,
                                      CodeGenOptLevel OptLevel);

    FunctionPass *createFTXT4KAddMemFencePass();
    void initializeFTXT4KAddMemFencePass(PassRegistry &);
} // namespace llvm
#endif //FTXT4K_H
