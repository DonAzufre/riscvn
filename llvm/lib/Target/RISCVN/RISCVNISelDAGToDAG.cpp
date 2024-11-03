#include "RISCVNISelDAGToDAG.h"

using namespace llvm;

#define DEBUG_TYPE "riscvn-isel"
#define PASS_NAME "RISCVN DAG->DAG Pattern Instruction Selection"

char RISCVNDAGToDAGISelLegacy::ID = 0;

void RISCVNDAGToDAGISel::Select(SDNode *Node) { SelectCode(Node); }

FunctionPass *llvm::createRISCVNISelDag(RISCVNTargetMachine &TM,
                                        CodeGenOptLevel OptLevel) {
  return new RISCVNDAGToDAGISelLegacy(TM, OptLevel);
}