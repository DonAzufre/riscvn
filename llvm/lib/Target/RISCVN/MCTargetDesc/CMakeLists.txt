add_llvm_component_library(LLVMRISCVNDesc
        RISCVNMCTargetDesc.cpp
        RISCVNInstPrinter.cpp
        RISCVNMCAsmInfo.cpp

        LINK_COMPONENTS
        MC
        RISCVNInfo
        Support
        TargetParser

        ADD_TO_COMPONENT
        RISCVN
        )