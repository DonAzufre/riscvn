add_llvm_component_library(LLVMRISCVNAsmParser
        RISCVNAsmParser.cpp

        LINK_COMPONENTS
        MC
        MCParser
        RISCVNDesc
        RISCVNInfo
        Support

        ADD_TO_COMPONENT
        RISCVN
        )