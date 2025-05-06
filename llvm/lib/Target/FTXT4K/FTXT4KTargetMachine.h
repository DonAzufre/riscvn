//
// Created by DonAzufre on 25-5-5.
//

#ifndef LLVM_FTXT4KTARGETMACHINE_H
#define LLVM_FTXT4KTARGETMACHINE_H

#include "FTXT4KSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

    class FTXT4KTargetMachine : public LLVMTargetMachine {
        std::unique_ptr<TargetLoweringObjectFile> TLOF;
        FTXT4KSubtarget Subtarget;

    public:
        FTXT4KTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                            StringRef FS, const TargetOptions &Options,
                            std::optional<Reloc::Model> RM,
                            std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                            bool JIT);

        const FTXT4KSubtarget *getSubtargetImpl() const { return &Subtarget; }
        const FTXT4KSubtarget *getSubtargetImpl(const Function &) const override {
            return &Subtarget;
        }

        TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

        TargetLoweringObjectFile *getObjFileLowering() const override {
            return TLOF.get();
        }
    };
} // namespace llvm

#endif // LLVM_FTXT4KTARGETMACHINE_H
