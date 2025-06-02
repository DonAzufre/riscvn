add_llvm_component_group(FTXT4K)

set(LLVM_TARGET_DEFINITIONS FTXT4K.td)

tablegen(LLVM FTXT4KGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM FTXT4KGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM FTXT4KGenCallingConv.inc -gen-callingconv)
tablegen(LLVM FTXT4KGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM FTXT4KGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM FTXT4KGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM FTXT4KGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM FTXT4KGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM FTXT4KGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM FTXT4KGenDisassemblerTables.inc -gen-disassembler)

add_public_tablegen_target(FTXT4KCommonTableGen)

add_llvm_target(FTXT4KCodeGen
        FTXT4KAsmPrinter.cpp
        FTXT4KFrameLowering.cpp
        FTXT4KInstrInfo.cpp
        FTXT4KISelDAGToDAG.cpp
        FTXT4KISelLowering.cpp
        FTXT4KRegisterInfo.cpp
        FTXT4KSubtarget.cpp
        FTXT4KTargetMachine.cpp
        FTXT4KMCInstLower.cpp
        FTXT4KFixInstConstrain.cpp

        LINK_COMPONENTS
        AsmPrinter
        CodeGen
        CodeGenTypes
        Core
        MC
        SelectionDAG
        FTXT4KDesc
        FTXT4KInfo
        Support
        Target
        TargetParser

        ADD_TO_COMPONENT
        FTXT4K
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)