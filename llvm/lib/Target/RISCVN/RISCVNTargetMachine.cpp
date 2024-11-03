#include "RISCVNTargetMachine.h"
#include "RISCVN.h"
#include "TargetInfo/RISCVNTargetInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"

#include <optional>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCVNTarget() {
  RegisterTargetMachine<RISCVNTargetMachine> X(getTheRISCVNTarget());
}

static StringRef computeDataLayout(const Triple &TT) {
  return "e-m:e-p:32:32-i32:32-n32-S32";
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

RISCVNTargetMachine::RISCVNTargetMachine(const Target &T, const Triple &TT,
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
class RISCVNPassConfig : public TargetPassConfig {
public:
  RISCVNPassConfig(RISCVNTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  RISCVNTargetMachine &getRISCVNTargetMachine() const {
    return getTM<RISCVNTargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *RISCVNTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new RISCVNPassConfig(*this, PM);
}

bool RISCVNPassConfig::addInstSelector() {
  addPass(createRISCVNISelDag(getRISCVNTargetMachine(), getOptLevel()));
  return false;
}