//
// Created by myrica on 4/16/25.
//

#ifndef LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNBASEINFO_H
#define LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNBASEINFO_H

namespace llvm {

namespace RISCVNII {

// machine op flags

enum {
  MO_None = 0,
  MO_CAll,
  MO_LO,
  MO_HI
};

}

namespace RISCVNOp {

enum OperandType : unsigned {
  OPERAND_SIMM12,
};

}

}

#endif //LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNBASEINFO_H
