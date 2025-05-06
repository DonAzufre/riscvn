//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KISelDAGToDAG.h"

using namespace llvm;

#define DEBUG_TYPE "riscvn-isel"
#define PASS_NAME "FTXT4K DAG->DAG Pattern Instruction Selection"

char FTXT4KDAGToDAGISelLegacy::ID = 0;

void FTXT4KDAGToDAGISel::Select(SDNode *Node) { SelectCode(Node); }

FunctionPass *llvm::createFTXT4KISelDag(FTXT4KTargetMachine &TM,
                                        CodeGenOptLevel OptLevel) {
    return new FTXT4KDAGToDAGISelLegacy(TM, OptLevel);
}