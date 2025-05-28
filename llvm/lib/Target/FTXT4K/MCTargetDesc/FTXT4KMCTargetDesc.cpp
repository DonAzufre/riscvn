//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KMCTargetDesc.h"
#include "FTXT4KInstPrinter.h"
#include "FTXT4KMCAsmInfo.h"
#include "TargetInfo/FTXT4KTargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "FTXT4KBaseInfo.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "FTXT4KGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "FTXT4KGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "FTXT4KGenRegisterInfo.inc"

static MCAsmInfo *createFTXT4KMCAsmInfo(const MCRegisterInfo &MRI,
                                        const Triple &TT,
                                        const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new FTXT4KMCAsmInfo(TT);
  return MAI;
}

static MCInstrInfo *createFTXT4KMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitFTXT4KMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createFTXT4KMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitFTXT4KMCRegisterInfo(X, FTXT4K::R63);
  return X;
}

static MCSubtargetInfo *
createFTXT4KMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  if (CPU.empty()) {
    CPU = "ftxt4k";
  }
  return createFTXT4KMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstPrinter *createFTXT4KMCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  return new FTXT4KInstPrinter(MAI, MII, MRI);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFTXT4KTargetMC() {
  for (Target *T : {&getTheFTXT4KTarget()}) {
    TargetRegistry::RegisterMCAsmInfo(*T, createFTXT4KMCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(*T, createFTXT4KMCInstrInfo);
    TargetRegistry::RegisterMCSubtargetInfo(*T, createFTXT4KMCSubtargetInfo);
    TargetRegistry::RegisterMCRegInfo(*T, createFTXT4KMCRegisterInfo);
    TargetRegistry::RegisterMCInstPrinter(*T, createFTXT4KMCInstPrinter);
  }
}