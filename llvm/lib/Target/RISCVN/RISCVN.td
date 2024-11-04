include "llvm/Target/Target.td"

include "RISCVNRegisterInfo.td"
include "RISCVNCallingConv.td"
include "RISCVNInstrInfo.td"


def : ProcessorModel<"riscvn", NoSchedModel, []>;


def RISCVNInstrInfo : InstrInfo;

def RISCVNAsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 1;
}

def RISCVN : Target {
  let InstructionSet = RISCVNInstrInfo;
  let AssemblyParsers = [RISCVNAsmParser];
}