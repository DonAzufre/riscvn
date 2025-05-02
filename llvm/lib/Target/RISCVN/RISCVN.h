#ifndef LLVM_LIB_TARGET_RISCVN_RISCVN_H
#define LLVM_LIB_TARGET_RISCVN_RISCVN_H

#include "llvm/PassRegistry.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class RISCVNTargetMachine;
class FunctionPass;

FunctionPass *createRISCVNISelDag(RISCVNTargetMachine &TM,
                                  CodeGenOptLevel OptLevel);
FunctionPass *createRISCVNExpandPseudoPass();

void initializeRISCVNExpandPseudoPass(PassRegistry &);
} // namespace llvm

constexpr bool isSimm12(int64_t v) { return v >= -2048 && v <= 2047; }

#endif
