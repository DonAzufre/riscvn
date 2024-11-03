#include "RISCVNISelLowering.h"
#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "RISCVNSubtarget.h"
#include "llvm/CodeGen/CallingConvLower.h"

using namespace llvm;

#define DEBUG_TYPE "RISCVN-lower"

#include "RISCVNGenCallingConv.inc"

RISCVNTargetLowering::RISCVNTargetLowering(const TargetMachine &TM,
                                           const RISCVNSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  // Set up the register classes.
  addRegisterClass(MVT::i32, &RISCVN::GPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());
}

SDValue RISCVNTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  return Chain;
}

SDValue
RISCVNTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                  bool IsVarArg,
                                  const SmallVectorImpl<ISD::OutputArg> &Outs,
                                  const SmallVectorImpl<SDValue> &OutVals,
                                  const SDLoc &DL, SelectionDAG &DAG) const {
  SmallVector<CCValAssign, 16> RVLocs;

  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeReturn(Outs, RetCC_RISCVN);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");
    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain;

  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  return DAG.getNode(RISCVNISD::RET_GLUE, DL, MVT::Other, RetOps);
}

const char *RISCVNTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch ((RISCVNISD::NodeType)Opcode) {
  case RISCVNISD::RET_GLUE:
    return "SXGPUISD::RET_GLUE";
  default:
    return nullptr;
  }
}