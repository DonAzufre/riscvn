def RetCC_RISCVN : CallingConv<[
  CCIfType<[i32], CCAssignToReg<[X10, X11]>>,
]>;

def CC_RISCVN : CallingConv<[
  // Promote i8/i16 args to i32.
  CCIfType<[i8, i16], CCPromoteToType<i32>>,

  CCIfType<[i32], CCAssignToReg<[X10, X11, X12, X13, X14, X15, X16, X17]>>,

  // Other arguments are passed on the stack, at least 4-byte-aligned.
  CCAssignToStack<4, 4>
]>;



def CSR_RISCVN : CalleeSavedRegs<(add X2, X8, X9, (sequence "X%d", 18, 27))>;

// todo: f regs
def CSR_RISCVN_CALLSAVE : CalleeSavedRegs<(add X1, X8, X9, (sequence "X%u", 18, 27))>;