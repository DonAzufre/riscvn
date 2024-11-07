add_llvm_component_library(LLVMRISCVNDisassembler
  RISCVNDisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  RISCVNDesc
  RISCVNInfo
  Support

  ADD_TO_COMPONENT
  RISCVN
  )