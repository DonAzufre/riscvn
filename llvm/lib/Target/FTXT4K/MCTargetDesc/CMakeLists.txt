add_llvm_component_library(LLVMFTXT4KDesc
        FTXT4KMCTargetDesc.cpp
        FTXT4KInstPrinter.cpp
        FTXT4KMCAsmInfo.cpp


        LINK_COMPONENTS
        MC
        FTXT4KInfo
        Support
        TargetParser

        ADD_TO_COMPONENT
        FTXT4K
)