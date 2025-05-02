add_llvm_component_group(RISCVN)

set(LLVM_TARGET_DEFINITIONS RISCVN.td)

tablegen(LLVM RISCVNGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM RISCVNGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM RISCVNGenCallingConv.inc -gen-callingconv)
tablegen(LLVM RISCVNGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM RISCVNGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM RISCVNGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM RISCVNGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM RISCVNGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM RISCVNGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM RISCVNGenDisassemblerTables.inc -gen-disassembler)

add_public_tablegen_target(RISCVNCommonTableGen)

add_llvm_target(RISCVNCodeGen
        RISCVNAsmPrinter.cpp
        RISCVNFrameLowering.cpp
        RISCVNInstrInfo.cpp
        RISCVNISelDAGToDAG.cpp
        RISCVNISelLowering.cpp
        RISCVNRegisterInfo.cpp
        RISCVNSubtarget.cpp
        RISCVNTargetMachine.cpp
        RISCVNMCInstLower.cpp
        RISCVNExpandPseudoInst.cpp

        LINK_COMPONENTS
        AsmPrinter
        CodeGen
        CodeGenTypes
        Core
        MC
        SelectionDAG
        RISCVNDesc
        RISCVNInfo
        Support
        Target
        TargetParser

        ADD_TO_COMPONENT
        RISCVN
        )

add_subdirectory(AsmParser) 
add_subdirectory(Disassembler)       
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)