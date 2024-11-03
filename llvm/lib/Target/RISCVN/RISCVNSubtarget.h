#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNSubtarget_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNSubtarget_H

#include "RISCVNFrameLowering.h"
#include "RISCVNISelLowering.h"
#include "RISCVNInstrInfo.h"
#include "RISCVNRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "RISCVNGenSubtargetInfo.inc"
//
namespace llvm {
class StringRef;

class RISCVNSubtarget : public RISCVNGenSubtargetInfo {
private:
  virtual void anchor();

  RISCVNFrameLowering FrameLowering;
  RISCVNInstrInfo InstrInfo;
  RISCVNTargetLowering TLInfo;
  RISCVNRegisterInfo RegInfo;
  SelectionDAGTargetInfo TSInfo;

public:
  RISCVNSubtarget(const Triple &TT, const std::string &CPU,
                  const std::string &FS, const TargetMachine &TM);

  RISCVNSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                   const TargetMachine &TM);

  const RISCVNInstrInfo *getInstrInfo() const override { return &InstrInfo; }

  const RISCVNFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }

  const RISCVNTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }

  const RISCVNRegisterInfo *getRegisterInfo() const override {
    return &RegInfo;
  }

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
};
} // namespace llvm

#endif