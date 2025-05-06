def RetCC_FTXT4K : CallingConv<[
  CCIfType<[i32], CCAssignToReg<[R63]>>,
]>;

def CSR_FTXT4K : CalleeSavedRegs<(add )>;