//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KISelDAGToDAG.h"

using namespace llvm;

#define DEBUG_TYPE "ftxt4k-isel"
#define PASS_NAME "FTXT4K DAG->DAG Pattern Instruction Selection"

char FTXT4KDAGToDAGISelLegacy::ID = 0;

void FTXT4KDAGToDAGISel::Select(SDNode *Node) {

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
            SDValue Imm = CurDAG->getTargetConstant(0, DL, MVT::i64);
            int FI = cast<FrameIndexSDNode>(Node)->getIndex();
            SDValue TFI = CurDAG->getTargetFrameIndex(FI, MVT::i64);
            ReplaceNode(Node,
                        CurDAG->getMachineNode(FTXT4K::SADDI, DL, MVT::i64, TFI, Imm));
            IsSelected = true;
            // report_fatal_error("ftxt4k: select frameindex not implemented.");
        } break;
    }

    if (IsSelected)
        return;

    SelectCode(Node);
}

FunctionPass *llvm::createFTXT4KISelDag(FTXT4KTargetMachine &TM,
                                        CodeGenOptLevel OptLevel) {
    return new FTXT4KDAGToDAGISelLegacy(TM, OptLevel);
}