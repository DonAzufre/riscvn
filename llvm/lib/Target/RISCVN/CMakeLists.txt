add_llvm_component_group(RISCVN)

add_llvm_target(RISCVNCodeGen
        RISCVNTargetMachine.cpp

        ADD_TO_COMPONENT
        RISCVN
        )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)