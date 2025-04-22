#include "RISCVNISelDAGToDAG.h"

#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define DEBUG_TYPE "riscvn-isel"
#define PASS_NAME "RISCVN DAG->DAG Pattern Instruction Selection"

char RISCVNDAGToDAGISelLegacy::ID = 0;

void RISCVNDAGToDAGISel::Select(SDNode *Node) {

  // If we have a custom node, we have already selected.
  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(dbgs() << "== "; Node->dump(CurDAG); dbgs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  SDLoc DL(Node);
  auto Opcode = Node->getOpcode();
  bool IsSelected = false;

  switch (Opcode) {
  default:
    break;
  case ISD::FrameIndex: {
    SDValue Imm = CurDAG->getTargetConstant(0, DL, MVT::i32);
    int FI = cast<FrameIndexSDNode>(Node)->getIndex();
    SDValue TFI = CurDAG->getTargetFrameIndex(FI, MVT::i32);
    ReplaceNode(Node,
                CurDAG->getMachineNode(RISCVN::ADDI, DL, MVT::i32, TFI, Imm));
    IsSelected = true;
  } break;
  }

  if (IsSelected)
    return;

  SelectCode(Node);
}
bool RISCVNDAGToDAGISel::SelectFrameAddrRegImm(SDValue Addr, SDValue &Base,
                                               SDValue &Offset) {
  if (SelectAddrFrameIndex(Addr, Base, Offset))
    return true;

  if (!CurDAG->isBaseWithConstantOffset(Addr))
    return false;

  if (auto *FIN = dyn_cast<FrameIndexSDNode>(Addr.getOperand(0))) {
    int64_t CVal = cast<ConstantSDNode>(Addr.getOperand(1))->getSExtValue();
    if (isInt<12>(CVal)) {
      Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), MVT::i32);
      Offset = CurDAG->getTargetConstant(CVal, SDLoc(Addr), MVT::i32);
      return true;
    }
  }
  return false;
}
bool RISCVNDAGToDAGISel::SelectAddrFrameIndex(SDValue Addr, SDValue &Base,
                                              SDValue &Offset) {
  if (auto *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), MVT::i32);
    Offset = CurDAG->getTargetConstant(0, SDLoc(Addr), MVT::i32);
    return true;
  }

  return false;
}

FunctionPass *llvm::createRISCVNISelDag(RISCVNTargetMachine &TM,
                                        CodeGenOptLevel OptLevel) {
  return new RISCVNDAGToDAGISelLegacy(TM, OptLevel);
}