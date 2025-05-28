//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KISelLowering.h"
#include "MCTargetDesc/FTXT4KMCTargetDesc.h"
#include "FTXT4KSubtarget.h"
#include "llvm/CodeGen/CallingConvLower.h"

using namespace llvm;

#define DEBUG_TYPE "FTXT4K-lower"

#include "FTXT4KGenCallingConv.inc"

MVT FTXT4KTargetLowering::getPointerTy(const DataLayout &DL, uint32_t AS) const {
  return MVT::i64;
}

void FTXT4KTargetLowering::ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results, SelectionDAG &DAG) const {
  SDLoc DL(N);
  switch (N->getOpcode()) {
    default:
      report_fatal_error("ftxt4k: replace node results failed.");
    case ISD::FrameIndex: {
      int FI = cast<FrameIndexSDNode>(N)->getIndex();

      SDValue TFI = DAG.getTargetFrameIndex(FI, MVT::i32);
      SDValue TFI_ext = DAG.getNode(ISD::ZERO_EXTEND, DL, MVT::i64, TFI);
      Results.push_back(TFI_ext);
    }
    break;
  }
}

SDValue FTXT4KTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
    default:
      report_fatal_error("ftxt4k: lower operation failed.");
    case ISD::FrameIndex: {
      int FI = cast<FrameIndexSDNode>(Op)->getIndex();
      SDValue TFI = DAG.getTargetFrameIndex(FI, MVT::i64);
      return TFI;
    }
    break;
  }
}

FTXT4KTargetLowering::FTXT4KTargetLowering(const TargetMachine &TM,
                                           const FTXT4KSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  // Set up the register classes.
  addRegisterClass(MVT::i64, &FTXT4K::GPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());



}

SDValue FTXT4KTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  return Chain;
}

SDValue
FTXT4KTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                  bool IsVarArg,
                                  const SmallVectorImpl<ISD::OutputArg> &Outs,
                                  const SmallVectorImpl<SDValue> &OutVals,
                                  const SDLoc &DL, SelectionDAG &DAG) const {
  SmallVector<CCValAssign, 16> RVLocs;

  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeReturn(Outs, RetCC_FTXT4K);

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

  return DAG.getNode(FTXT4KISD::RET_GLUE, DL, MVT::Other, RetOps);
}

const char *FTXT4KTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch ((FTXT4KISD::NodeType)Opcode) {
  case FTXT4KISD::RET_GLUE:
    return "FTXT4KISD::RET_GLUE";
  default:
    return nullptr;
  }
}