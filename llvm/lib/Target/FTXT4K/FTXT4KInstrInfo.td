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

class FTXT4KSImmOp<int bitsNum> : FTXT4KOp {
    let ParserMatchClass = SImmAsmOperand<bitsNum>;
    let EncoderMethod = "getImmOpValue";
    let DecoderMethod = "decodeSImmOperand<" # bitsNum # ">";
    let OperandType = "OPERAND_SIMM" # bitsNum;
}

class FTXT4KSImmLeafOp<int bitsNum> :
    FTXT4KSImmOp<bitsNum>, ImmLeaf<i32, "return isInt<" # bitsNum # ">(Imm);">;
    
def simm64 : FTXT4KSImmLeafOp<64> {
      let MCOperandPredicate = [{
        int64_t Imm;
        if (MCOp.evaluateAsConstantImm(Imm))
          return isInt<64>(Imm);
        return MCOp.isBareSymbolRef();
      }];
}

def SMOVI : Ins80Mov<
    0b01, // .M1
    (outs GPR:$dst),
    (ins simm64:$imm64),
    "SMOVI\t.M1\t$imm64,\t$dst",
    [(set i64:$dst, imm:$imm64)]>;


def retglue : SDNode<"FTXT4KISD::RET_GLUE", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

let isBarrier = 1, isReturn = 1, isTerminator = 1 in {
  def PseudoRET : Ins40Pseudo<
              (outs),
              (ins),
              "<return part>",
              [(retglue)]>;
}