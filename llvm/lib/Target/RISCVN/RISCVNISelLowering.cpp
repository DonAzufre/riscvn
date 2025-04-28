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

  setBooleanContents(ZeroOrOneBooleanContent);

  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
  // setOperationAction(ISD::SELECT, MVT::i32, Custom);

  // setOperationAction(ISD::SELECT_CC, MVT::i32, Expand);
}
MachineBasicBlock *RISCVNTargetLowering::EmitInstrWithCustomInserter(
    MachineInstr &MI, MachineBasicBlock *MBB) const {
  switch (MI.getOpcode()) {
  default:
    report_fatal_error("riscvn: emit with custon inserter not implemented.");
  case RISCVN::riscvn_select:
    return emitSelect(MI, MBB);
  }
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

SDValue RISCVNTargetLowering::LowerCallResult(
    SDValue Chain, SDValue InGlue, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeCallResult(Ins, RetCC_RISCVN);

  // Copy all of the result registers out of their specified physreg.
  for (unsigned i = 0, e = RVLocs.size(); i != e; ++i) {
    CCValAssign &VA = RVLocs[i];
    EVT CopyVT = VA.getLocVT();

    /// ??? is this correct?
    Chain = DAG.getCopyFromReg(Chain, DL, VA.getLocReg(), CopyVT, InGlue)
                .getValue(1);
    SDValue Val = Chain.getValue(0);

    if (VA.isExtInLoc() && VA.getValVT().getScalarType() == MVT::i1)
      Val = DAG.getNode(ISD::TRUNCATE, DL, VA.getValVT(), Val);

    InGlue = Chain.getValue(2);
    InVals.push_back(Val);
  }

  return Chain;
}

SDValue
RISCVNTargetLowering::LowerCall(CallLoweringInfo &CLI,
                                SmallVectorImpl<SDValue> &InVals) const {

  SelectionDAG &DAG = CLI.DAG;
  SDLoc &DL = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  bool &IsTailCall = CLI.IsTailCall;
  CallingConv::ID CallConv = CLI.CallConv;
  bool IsVarArg = CLI.IsVarArg; // Varargs calls
  MachineFunction &MF = DAG.getMachineFunction();

  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeCallOperands(Outs, CC_RISCVN);

  assert(!IsTailCall);
  assert(!IsVarArg);

  unsigned NumBytes = CCInfo.getStackSize();

  // todo: Create local copies for byval args

  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, DL);

  SmallVector<std::pair<Register, SDValue>> RegsToPass;
  SmallVector<SDValue> MemOpChains;
  SDValue StackPtr;

  const RISCVNRegisterInfo *RegInfo = Subtarget->getRegisterInfo();
  for (unsigned i = 0, e = ArgLocs.size(); i < e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    EVT RegVT = VA.getLocVT();
    SDValue ArgValue = OutVals[i];
    ISD::ArgFlagsTy Flags = Outs[i].Flags;
    assert(!Flags.isInAlloca() && !Flags.isByVal());

    switch (VA.getLocInfo()) {
    default:
      llvm_unreachable("Unknown loc info!");
    case CCValAssign::Full:
      break;
    }

    if (VA.isRegLoc())
      RegsToPass.emplace_back(VA.getLocReg(), ArgValue);
    else
      report_fatal_error("args of memloc not implemented");
  }

  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, MemOpChains);

  SDValue InGlue;
  for (unsigned i = 0, e = RegsToPass.size(); i < e; ++i) {
    Chain = DAG.getCopyToReg(Chain, DL, RegsToPass[i].first,
                             RegsToPass[i].second, InGlue);
    InGlue = Chain.getValue(1);
  }

  assert(Callee.getOpcode() == ISD::GlobalAddress);

  auto *G = cast<GlobalAddressSDNode>(Callee);
  const GlobalValue *GV = G->getGlobal();
  if (!GV->hasDLLImportStorageClass()) {
    unsigned char OpFlags = Subtarget->classifyGlobalFunctionReference(GV);

    Callee = DAG.getTargetGlobalAddress(
        GV, DL, getPointerTy(DAG.getDataLayout()), G->getOffset(), OpFlags);
  }

  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;

  Ops.push_back(Chain);
  Ops.push_back(Callee);
  for (unsigned i = 0, e = RegsToPass.size(); i < e; ++i) {
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
                                  RegsToPass[i].second.getValueType()));
  }

  const uint32_t *Mask = RegInfo->getCallPreservedMask(MF, CallConv);
  assert(Mask && "Missing call preserved mask for calling convention");

  Ops.push_back(DAG.getRegisterMask(Mask));
  if (InGlue.getNode())
    Ops.push_back(InGlue);

  Chain = DAG.getNode(RISCVNISD::CALL, DL, NodeTys, Ops);
  InGlue = Chain.getValue(1);

  // callee pop

  // size2: callee pop bytes
  Chain = DAG.getCALLSEQ_END(Chain, NumBytes, 0, InGlue, DL);
  InGlue = Chain.getValue(1);

  return LowerCallResult(Chain, InGlue, CallConv, IsVarArg, Ins, DL, DAG,
                         InVals);

  report_fatal_error("riscvn: LowerCall not implemented");
}

SDValue RISCVNTargetLowering::LowerGlobalAddress(SDValue Op,
                                                 SelectionDAG &DAG) const {
  assert(!isPositionIndependent());

  auto DL = SDLoc(Op);
  auto Ty = Op.getValueType();
  auto N = cast<GlobalAddressSDNode>(Op);
  const auto GV = N->getGlobal();
  auto Offset = N->getOffset();

  auto AddrHi = DAG.getTargetGlobalAddress(GV, DL, Ty, 0, RISCVNII::MO_HI);
  auto AddrLo = DAG.getTargetGlobalAddress(GV, DL, Ty, 0, RISCVNII::MO_LO);
  auto MNHi = DAG.getNode(RISCVNISD::HI, DL, Ty, AddrHi);
  auto MNLo = DAG.getNode(RISCVNISD::ADD_LO, DL, Ty, MNHi, AddrLo);

  if (Offset == 0)
    return MNLo;
  else
    return DAG.getNode(ISD::ADD, DL, Ty, MNLo,
                       DAG.getConstant(Offset, DL, MVT::i32));
}
SDValue RISCVNTargetLowering::LowerSelect(SDValue Op, SelectionDAG &DAG) const {

  report_fatal_error("riscvn: unimplemented lower select");
  SDLoc DL(Op);
  SDValue Cond = Op.getOperand(0);
  SDValue TrueVal = Op.getOperand(1);
  SDValue FalseVal = Op.getOperand(2);

  // if (Cond.getValueType() != MVT::i1) {
  //   Cond = DAG.getNode(ISD::TRUNCATE, DL, MVT::i1, Cond);
  // }

  Cond = DAG.getSetCC(DL, MVT::i1, Cond,
                      DAG.getConstant(0, DL, Cond.getValueType()), ISD::SETNE);

  Cond = DAG.getNode(ISD::SIGN_EXTEND, DL, MVT::i32, Cond);
  SDValue NotCond = DAG.getNode(ISD::XOR, DL, MVT::i32, Cond,
                                DAG.getConstant(-1, DL, MVT::i32));
  SDValue TruePart = DAG.getNode(ISD::AND, DL, MVT::i32, TrueVal, Cond);
  SDValue FalsePart = DAG.getNode(ISD::AND, DL, MVT::i32, FalseVal, NotCond);

  return DAG.getNode(ISD::OR, DL, MVT::i32, TruePart, FalsePart);
}
MachineBasicBlock *RISCVNTargetLowering::emitSelect(MachineInstr &MI,
                                                    MachineBasicBlock *BB) {
  const TargetInstrInfo &TII = *BB->getParent()->getSubtarget().getInstrInfo();
  DebugLoc DL = MI.getDebugLoc();

  assert(MI.getNumOperands() == 4);

  Register Dst = MI.getOperand(0).getReg();
  Register Cond = MI.getOperand(1).getReg();
  Register TVal = MI.getOperand(2).getReg();
  Register FVal = MI.getOperand(3).getReg();

  const BasicBlock *LLVM_BB = BB->getBasicBlock();
  MachineFunction::iterator It = ++BB->getIterator();

  MachineBasicBlock *thisMBB = BB;
  MachineFunction *F = BB->getParent();
  MachineBasicBlock *copyMBB = F->CreateMachineBasicBlock(LLVM_BB);
  MachineBasicBlock *sinkMBB = F->CreateMachineBasicBlock(LLVM_BB);
  F->insert(It, copyMBB);
  F->insert(It, sinkMBB);

  sinkMBB->splice(sinkMBB->begin(), thisMBB,
                  std::next(MachineBasicBlock::iterator(MI)), thisMBB->end());
  sinkMBB->transferSuccessorsAndUpdatePHIs(thisMBB);

  // in thisMbb:
  thisMBB->addSuccessor(copyMBB);
  thisMBB->addSuccessor(sinkMBB);

  // bne cond, 0, copyMBB
  BuildMI(thisMBB, DL, TII.get(RISCVN::BNE))
      .addReg(Cond)
      .addReg(RISCVN::X0)
      .addMBB(copyMBB);
  // // addi res, falseValue, 0
  // BuildMI(thisMBB, DL, TII.get(RISCVN::ADDI), Dst).addReg(FVal).addImm(0);
  // // j sinkMBB
  // BuildMI(thisMBB, DL,
  // TII.get(RISCVN::JAL)).addReg(RISCVN::X0).addMBB(sinkMBB);

  // in copyMBB:
  copyMBB->addSuccessor(sinkMBB);
  // // addi res, trueValue, 0
  // BuildMI(copyMBB, DL, TII.get(RISCVN::ADDI), Dst).addReg(TVal).addImm(0);
  // // j sinkMbb
  // BuildMI(copyMBB, DL, TII.get(RISCVN::)).addReg(RISCVN::X0).addMBB(sinkMBB);

  BuildMI(*sinkMBB, sinkMBB->begin(), DL, TII.get(RISCVN::PHI), Dst)
      .addReg(TVal)
      .addMBB(copyMBB)
      .addReg(FVal)
      .addMBB(thisMBB);

  MI.eraseFromParent();

  F->getProperties().reset(MachineFunctionProperties::Property::NoPHIs);

  return sinkMBB;
}

SDValue RISCVNTargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("riscvn: unimplemented operand");
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
    // case ISD::SELECT:
    //   return LowerSelect(Op, DAG);
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
  case RISCVNISD::CALL:
    return "RISCVNISD::CALL";
  default:
    llvm_unreachable("riscvn: unreachable node name");
  }
}