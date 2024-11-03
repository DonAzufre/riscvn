#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNTARGETMACHINE_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNTARGETMACHINE_H

#include "RISCVNSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class RISCVNTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  RISCVNSubtarget Subtarget;

public:
  RISCVNTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      std::optional<Reloc::Model> RM,
                      std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                      bool JIT);

  const RISCVNSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const RISCVNSubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
} // namespace llvm

#endif