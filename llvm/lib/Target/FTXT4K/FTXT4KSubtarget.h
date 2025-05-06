//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KSUBTARGET_H
#define FTXT4KSUBTARGET_H

#include "FTXT4KFrameLowering.h"
#include "FTXT4KISelLowering.h"
#include "FTXT4KInstrInfo.h"
#include "FTXT4KRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "FTXT4KGenSubtargetInfo.inc"
//
namespace llvm {
    class StringRef;

    class FTXT4KSubtarget : public FTXT4KGenSubtargetInfo {
    private:
        virtual void anchor();

        FTXT4KFrameLowering FrameLowering;
        FTXT4KInstrInfo InstrInfo;
        FTXT4KTargetLowering TLInfo;
        FTXT4KRegisterInfo RegInfo;
        SelectionDAGTargetInfo TSInfo;

    public:
        FTXT4KSubtarget(const Triple &TT, const std::string &CPU,
                        const std::string &FS, const TargetMachine &TM);

        FTXT4KSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                         const TargetMachine &TM);

        const FTXT4KInstrInfo *getInstrInfo() const override { return &InstrInfo; }

        const FTXT4KFrameLowering *getFrameLowering() const override {
            return &FrameLowering;
        }

        const FTXT4KTargetLowering *getTargetLowering() const override {
            return &TLInfo;
        }

        const FTXT4KRegisterInfo *getRegisterInfo() const override {
            return &RegInfo;
        }

        const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
            return &TSInfo;
        }

        void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
    };
} // namespace llvm

#endif //FTXT4KSUBTARGET_H
