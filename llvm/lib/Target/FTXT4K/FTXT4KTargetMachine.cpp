//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KTargetMachine.h"
#include "FTXT4K.h"
#include "TargetInfo/FTXT4KTargetInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"

#include <optional>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFTXT4KTarget() {
  RegisterTargetMachine<FTXT4KTargetMachine> X(getTheFTXT4KTarget());
}

static StringRef computeDataLayout(const Triple &TT) {
  return "e-m:e-p:32:32-i32:32-n32-S32";
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

FTXT4KTargetMachine::FTXT4KTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         std::optional<Reloc::Model> RM,
                                         std::optional<CodeModel::Model> CM,
                                         CodeGenOptLevel OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

namespace {
class FTXT4KPassConfig : public TargetPassConfig {
public:
  FTXT4KPassConfig(FTXT4KTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

protected:
  void addPreEmitPass() override;

public:
  FTXT4KTargetMachine &getFTXT4KTargetMachine() const {
    return getTM<FTXT4KTargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *FTXT4KTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new FTXT4KPassConfig(*this, PM);
}

void FTXT4KPassConfig::addPreEmitPass() {
  addPass(createFTXT4KAddMemFencePass());
}

bool FTXT4KPassConfig::addInstSelector() {
  addPass(createFTXT4KISelDag(getFTXT4KTargetMachine(), getOptLevel()));
  return false;
}
