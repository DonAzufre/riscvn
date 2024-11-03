def RetCC_RISCVN : CallingConv<[
  CCIfType<[i32], CCAssignToReg<[X10, X11]>>,
]>;