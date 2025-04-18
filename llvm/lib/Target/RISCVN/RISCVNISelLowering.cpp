#include "RISCVNISelLowering.h"
#include "MCTargetDesc/RISCVNBaseInfo.h"
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

  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
}

SDValue RISCVNTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  SmallVector<CCValAssign, 8> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_RISCVN);

  for (unsigned I = 0, E = ArgLocs.size(); I != E; ++I) {
    SDValue ArgValue;
    CCValAssign &VA = ArgLocs[I];
    EVT LocVT = VA.getLocVT();
    if (VA.isRegLoc()) {
      // Arguments passed in registers
      const TargetRegisterClass *RC;
      switch (LocVT.getSimpleVT().SimpleTy) {
      default:
        // Integers smaller than i64 should be promoted
        // to i32.
        llvm_unreachable("Unexpected argument type");
      case MVT::i32:
        RC = &RISCVN::GPRRegClass;
        break;
      }

      Register VReg = MRI.createVirtualRegister(RC);
      MRI.addLiveIn(VA.getLocReg(), VReg);
      ArgValue = DAG.getCopyFromReg(Chain, DL, VReg, LocVT);

      // If this is an 8/16-bit value, it is really
      // passed promoted to 32 bits. Insert an
      // assert[sz]ext to capture this, then truncate to
      // the right size.
      if (VA.getLocInfo() == CCValAssign::SExt)
        ArgValue = DAG.getNode(ISD::AssertSext, DL, LocVT, ArgValue,
                               DAG.getValueType(VA.getValVT()));
      else if (VA.getLocInfo() == CCValAssign::ZExt)
        ArgValue = DAG.getNode(ISD::AssertZext, DL, LocVT, ArgValue,
                               DAG.getValueType(VA.getValVT()));

      if (VA.getLocInfo() != CCValAssign::Full)
        ArgValue = DAG.getNode(ISD::TRUNCATE, DL, VA.getValVT(), ArgValue);

      InVals.push_back(ArgValue);
    } else {
      assert(VA.isMemLoc() && "Argument not register or memory");
      llvm_unreachable("RISCVN - LowerFormalArguments - "
                       "Memory argument not implemented");
    }
  }

  if (IsVarArg) {
    llvm_unreachable("RISCVN - LowerFormalArguments - "
                     "VarArgs not Implemented");
  }

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

SDValue RISCVNTargetLowering::LowerGlobalAddress(SDValue Op,
                                                 SelectionDAG &DAG) const {
  assert(!isPositionIndependent());

  auto DL = SDLoc(Op);
  auto Ty = Op.getValueType();
  auto N = cast<GlobalAddressSDNode>(Op);
  const auto GV = N->getGlobal();

  auto AddrHi = DAG.getTargetGlobalAddress(GV, DL, Ty, 0, RISCVNII::MO_HI);
  auto AddrLo = DAG.getTargetGlobalAddress(GV, DL, Ty, 0, RISCVNII::MO_LO);
  auto MNHi = DAG.getNode(RISCVNISD::HI, DL, Ty, AddrHi);

  return DAG.getNode(RISCVNISD::ADD_LO, DL, Ty, MNHi, AddrLo);
}

SDValue RISCVNTargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("riscvn: unimplemented operand");
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  }
  return TargetLowering::LowerOperation(Op, DAG);
}

const char *RISCVNTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (static_cast<RISCVNISD::NodeType>(Opcode)) {
  case RISCVNISD::RET_GLUE:
    return "RISCVNISD::RET_GLUE";
  case RISCVNISD::HI:
    return "RISCVNISD::HI";
  case RISCVNISD::ADD_LO:
    return "RISCVNISD::ADD_LO";
  default:
    llvm_unreachable("riscvn: unreachable node name");
  }
}