#ifndef LLVM_LIB_TARGET_RISCVN_RISCVN_H
#define LLVM_LIB_TARGET_RISCVN_RISCVN_H

#include "llvm/Target/TargetMachine.h"

namespace llvm {

class RISCVNTargetMachine;
class FunctionPass;

FunctionPass *createRISCVNISelDag(RISCVNTargetMachine &TM,
                                  CodeGenOptLevel OptLevel);

} // namespace llvm

#endif
