//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KISELLOWERING_H
#define FTXT4KISELLOWERING_H


#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
    class FTXT4KSubtarget;

    namespace FTXT4KISD {
        enum NodeType : unsigned { FIRST_NUMBER = ISD::BUILTIN_OP_END, RET_GLUE };
    }

    class FTXT4KTargetLowering : public TargetLowering {
    public:
        MVT getPointerTy(const DataLayout &DL, uint32_t AS) const override;

        void ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results, SelectionDAG &DAG) const override;

    private:
        const FTXT4KSubtarget* Subtarget;

    public:
        SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

    public:
        explicit FTXT4KTargetLowering(const TargetMachine &TM,
                                      const FTXT4KSubtarget &STI);

        const char *getTargetNodeName(unsigned Opcode) const override;

    private:
        SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                                     bool IsVarArg,
                                     const SmallVectorImpl<ISD::InputArg> &Ins,
                                     const SDLoc &DL, SelectionDAG &DAG,
                                     SmallVectorImpl<SDValue> &InVals) const override;

        SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                            const SmallVectorImpl<ISD::OutputArg> &Outs,
                            const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                            SelectionDAG &DAG) const override;
    };

}

#endif //FTXT4KISELLOWERING_H
