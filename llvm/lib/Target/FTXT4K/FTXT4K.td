include "llvm/Target/Target.td"

include "FTXT4KRegisterInfo.td"
include "FTXT4KCallingConv.td"
include "FTXT4KInstrInfo.td"

def : ProcessorModel<"ftxt4k", NoSchedModel, []>;


def FTXT4KInstrInfo : InstrInfo;

def FTXT4K : Target {
  let InstructionSet = FTXT4KInstrInfo;
}