#include "RISCVNMCTargetDesc.h"
#include "RISCVNInstPrinter.h"
#include "RISCVNMCAsmInfo.h"
#include "TargetInfo/RISCVNTargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "RISCVNGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "RISCVNGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "RISCVNGenRegisterInfo.inc"

static MCAsmInfo *createRISCVNMCAsmInfo(const MCRegisterInfo &MRI,
                                        const Triple &TT,
                                        const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new RISCVNMCAsmInfo(TT);
  return MAI;
}

static MCInstrInfo *createRISCVNMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitRISCVNMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createRISCVNMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitRISCVNMCRegisterInfo(X, RISCVN::X1);
  return X;
}

static MCSubtargetInfo *
createRISCVNMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  if (CPU.empty()) {
    CPU = "riscvn";
  }
  return createRISCVNMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstPrinter *createRISCVNMCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  return new RISCVNInstPrinter(MAI, MII, MRI);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCVNTargetMC() {
  for (Target *T : {&getTheRISCVNTarget()}) {
    TargetRegistry::RegisterMCAsmInfo(*T, createRISCVNMCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(*T, createRISCVNMCInstrInfo);
    TargetRegistry::RegisterMCSubtargetInfo(*T, createRISCVNMCSubtargetInfo);
    TargetRegistry::RegisterMCRegInfo(*T, createRISCVNMCRegisterInfo);
    TargetRegistry::RegisterMCInstPrinter(*T, createRISCVNMCInstPrinter);
  }
}