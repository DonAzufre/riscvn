#ifndef LLVM_LIB_TARGET_RISCVN_RISCVNISELLOWERING_H
#define LLVM_LIB_TARGET_RISCVN_RISCVNISELLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
class RISCVNSubtarget;
namespace RISCVNISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  RET_GLUE,
  HI,
  ADD_LO,
  CALL,
};
}

class RISCVNTargetLowering : public TargetLowering {
  const RISCVNSubtarget *Subtarget;

public:
  explicit RISCVNTargetLowering(const TargetMachine &TM,
                                const RISCVNSubtarget &STI);
  MachineBasicBlock *
  EmitInstrWithCustomInserter(MachineInstr &MI,
                              MachineBasicBlock *MBB) const override;

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

  SDValue LowerCallResult(SDValue Chain, SDValue InGlue,
                        CallingConv::ID CallConv, bool IsVarArg,
                        const SmallVectorImpl<ISD::InputArg> &Ins,
                        const SDLoc &DL, SelectionDAG &DAG,
                        SmallVectorImpl<SDValue> &InVals) const;

public:
  SDValue LowerCall(CallLoweringInfo &,
                    SmallVectorImpl<SDValue> &) const override;

private:
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSelect(SDValue Op, SelectionDAG &DAG) const;

  static MachineBasicBlock *emitSelect(MachineInstr &MI, MachineBasicBlock *BB);

public:
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
};
} // namespace llvm

#endif