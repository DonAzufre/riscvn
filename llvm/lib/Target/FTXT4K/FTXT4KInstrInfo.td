include "FTXT4KInstrFormats.td"

class FTXT4KOp : Operand<i64> {
    let OperandNamespace = "FTXT4KOp";
}

class ImmAsmOperand<string prefix, int width, string suffix> : AsmOperandClass {
    let Name = prefix # "Imm" # width # suffix;
    let RenderMethod = "addImmOperands";
    let DiagnosticType = !strconcat("Invalid", Name);
}

class SImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"S", width, suffix> {
}

class UImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"U", width, suffix> {
}

class FTXT4KSImmOp<int bitsNum> : FTXT4KOp {
    let ParserMatchClass = SImmAsmOperand<bitsNum>;
    let EncoderMethod = "getImmOpValue";
    let DecoderMethod = "decodeSImmOperand<" # bitsNum # ">";
    let OperandType = "OPERAND_SIMM" # bitsNum;
}

class FTXT4KUImmOp<int bitsNum> : FTXT4KOp {
    let ParserMatchClass = UImmAsmOperand<bitsNum>;
    let EncoderMethod = "getUImmOpValue";
    let DecoderMethod = "decodeUImmOperand<" # bitsNum # ">";
    let OperandType = "OPERAND_UIMM" # bitsNum;
}

class FTXT4KSImmLeafOp<int bitsNum> :
    FTXT4KSImmOp<bitsNum>, ImmLeaf<i64, "return isInt<" # bitsNum # ">(Imm);">;

class FTXT4KUImmLeafOp<int bitsNum> :
    FTXT4KUImmOp<bitsNum>, ImmLeaf<i64, "return isUInt<" # bitsNum # ">(Imm);">;

def simm64 : FTXT4KSImmLeafOp<64> {
      let MCOperandPredicate = [{
        int64_t Imm;
        if (MCOp.evaluateAsConstantImm(Imm))
          return isInt<64>(Imm);
        return MCOp.isBareSymbolRef();
      }];
}

def scst6 : FTXT4KSImmLeafOp<6> {
      let MCOperandPredicate = [{
        int64_t Imm;
        if (MCOp.evaluateAsConstantImm(Imm))
          return isInt<6>(Imm);
        return MCOp.isBareSymbolRef();
      }];
}

def ucst18 : FTXT4KUImmLeafOp<18> {
      let MCOperandPredicate = [{
        int64_t Imm;
        if (MCOp.evaluateAsConstantImm(Imm))
          return isUInt<18>(Imm);
        return MCOp.isBareSymbolRef();
      }];
}

def SMOVI : Ins80Mov<
    0b01, // .M1
    (outs GPR:$dst),
    (ins simm64:$imm64),
    "SMOVI\t.M1\t$imm64,\t$dst",
    [(set i64:$dst, imm:$imm64)]>;

let mayStore = true, mayLoad = false in
def SSTW : Ins40LS<
    0b0110,
    (outs ),
    (ins GPR:$dst, SAR_40LS:$ar, ucst18:$ucst18),
    "SSTW\t$dst,\t*+$ar[$ucst18]",
    []>;

let mayStore = false, mayLoad = true in
def SLDW : Ins40LS<
    0b0010,
    (outs GPR:$dst),
    (ins SAR_40LS:$ar, ucst18:$ucst18),
    "SLDW\t*+$ar[$ucst18],\t$dst",
    []>;

def SADDI : Ins40SD<
    0b0100101000,
    0b001, // .M1
    (outs GPR:$dst),
    (ins scst6:$src1, GPR:$src2),
    "SADD\t.M1\t$src1,\t$src2,\t$dst",
    []>;

def SADD : Ins40SD<
    0b0100111000,
    0b001, // .M1
    (outs GPR:$dst),
    (ins GPR:$src1, GPR:$src2),
    "SADD\t.M1\t$src1,\t$src2,\t$dst",
    []>;

def SMFENCE : Ins40SD<
    0b0101000000,
    0b101, // no unit
    (outs ),
    (ins ),
    "SMFENCE",
    []>;


def : Pat<(add GPR:$a, GPR:$b), (SADD GPR:$a, GPR:$b)>;

def SMVAGA : Ins40SD<
    0b0111010000,
    0b001, // .M1
    (outs SAR:$dst),
    (ins GPR:$src1),
    "SMVAGA\t.M1\t$src1,\t$dst",
    []>;

def : Pat<(store i64:$val, GPR:$base), (SSTW GPR:$val, (SMVAGA GPR:$base), 0)>;
def : Pat<(load GPR:$base), (SLDW (SMVAGA GPR:$base), 0)>;

def retglue : SDNode<"FTXT4KISD::RET_GLUE", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

let isBarrier = 1, isReturn = 1, isTerminator = 1 in {
  def PseudoRET : Ins40Pseudo<
              (outs),
              (ins),
              "; return inst should be here",
              [(retglue)]>;
}