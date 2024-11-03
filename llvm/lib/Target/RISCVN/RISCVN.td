include "llvm/Target/Target.td"

include "RISCVNRegisterInfo.td"
include "RISCVNCallingConv.td"
include "RISCVNInstrInfo.td"


def : ProcessorModel<"riscvn", NoSchedModel, []>;


def RISCVNInstrInfo : InstrInfo;

def RISCVN : Target {
  let InstructionSet = RISCVNInstrInfo;
}