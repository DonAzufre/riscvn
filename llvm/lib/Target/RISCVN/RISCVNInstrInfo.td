include "RISCVNInstrFormats.td"


class RISCVNOp : Operand<i32> {
    let OperandNamespace = "RISCVNOp";
}

class ImmAsmOperand<string prefix, int width, string suffix> : AsmOperandClass {
    let Name = prefix # "Imm" # width # suffix;
    let RenderMethod = "addImmOperands";
    let DiagnosticType = !strconcat("Invalid", Name);
}

class SImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"S", width, suffix> {
}

class RISCVNSImmOp<int bitsNum> : RISCVNOp {
    let ParserMatchClass = SImmAsmOperand<bitsNum>;
    let EncoderMethod = "getImmOpValue";
    let DecoderMethod = "decodeSImmOperand<" # bitsNum # ">";
    let OperandType = "OPERAND_SIMM" # bitsNum;
}

class RISCVNSImmLeafOp<int bitsNum> :
    RISCVNSImmOp<bitsNum>, ImmLeaf<i32, "return isInt<" # bitsNum # ">(Imm);">;

def simm12 : RISCVNSImmLeafOp<12> {
      let MCOperandPredicate = [{
        int64_t Imm;
        if (MCOp.evaluateAsConstantImm(Imm))
          return isInt<12>(Imm);
        return MCOp.isBareSymbolRef();
      }];
}

def retglue : SDNode<"RISCVNISD::RET_GLUE", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;


def ADDI : I<OPC_ADDI,
            (outs GPR:$rd),
            (ins GPR:$rs1, simm12:$imm12),
            "addi $rd, $rs1, $imm12",
            []>;

let mayStore = false, mayLoad = true in {
    def LW : I<OPC_LW,
                (outs GPR:$rd),
                (ins GPR:$rs1, simm12:$imm12),
                "lw $rd, ${imm12}(${rs1})",
                []>;
}

def : Pat<(i32 (simm12:$imm12)), (ADDI X0, simm12:$imm12)>;
def : Pat<(add GPR:$val, simm12:$imm12), (ADDI GPR:$val, simm12:$imm12)>;

def : Pat<(load GPR:$addr), (LW GPR:$addr, 0)>;
def : Pat<(load tframeindex:$addr), (LW GPR:$addr, 0)>;

let isCall = 1 in {
  def JALR : I<OPC_JALR,
              (outs GPR:$rd),
              (ins GPR:$rs1, i32imm:$imm12),
              "jalr $rd, $rs1, $imm12",
              []>;
}

def ADD : R<OPC_ADD,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "add $rd, $rs1, $rs2",
            []>;

def SUB : R<OPC_SUB,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "sub $rd, $rs1, $rs2",
            []>;

def SLL : R<OPC_SLL,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "sll $rd, $rs1, $rs2",
            []>;

def SRL : R<OPC_SRL,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "srl $rd, $rs1, $rs2",
            []>;

def SRA : R<OPC_SRA,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "sra $rd, $rs1, $rs2",
            []>;

def SLT : R<OPC_SLT,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "slt $rd, $rs1, $rs2",
            []>;

def SLTU : R<OPC_SLTU,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "sltu $rd, $rs1, $rs2",
            []>;

def AND : R<OPC_AND,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "and $rd, $rs1, $rs2",
            []>;

def OR : R<OPC_OR,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "or $rd, $rs1, $rs2",
            []>;

def XOR : R<OPC_XOR,
            (outs GPR:$rd),
            (ins GPR:$rs1, GPR:$rs2),
            "xor $rd, $rs1, $rs2",
            []>;


def : Pat<(add GPR:$a, GPR:$b), (ADD GPR:$a, GPR:$b)>;
def : Pat<(sub GPR:$a, GPR:$b), (SUB GPR:$a, GPR:$b)>;
def : Pat<(shl GPR:$a, GPR:$b), (SLL GPR:$a, GPR:$b)>;
def : Pat<(srl GPR:$a, GPR:$b), (SRL GPR:$a, GPR:$b)>;
def : Pat<(sra GPR:$a, GPR:$b), (SRA GPR:$a, GPR:$b)>;
def : Pat<(and GPR:$a, GPR:$b), (AND GPR:$a, GPR:$b)>;
def : Pat<(or GPR:$a, GPR:$b), (OR GPR:$a, GPR:$b)>;
def : Pat<(xor GPR:$a, GPR:$b), (XOR GPR:$a, GPR:$b)>;
def : Pat<(setlt GPR:$a, GPR:$b), (SLT GPR:$a, GPR:$b)>;
def : Pat<(setgt GPR:$a, GPR:$b), (SLT GPR:$b, GPR:$a)>;
def : Pat<(setult GPR:$a, GPR:$b), (SLTU GPR:$a, GPR:$b)>;
def : Pat<(setugt GPR:$a, GPR:$b), (SLTU GPR:$b, GPR:$a)>;


let isBranch = true in {
    def JAL : J<OPC_JAL,
                (outs ),
                (ins GPR:$rd, jmptarget:$imm20),
                "jal $rd, $imm20",
                []>;
}

def : Pat<(br bb:$addr), (JAL X0, jmptarget:$addr)>;

let mayLoad = false, mayStore = true, imm12 = 0 in {
    def SW : S<OPC_SW,
                (outs ),
                (ins GPR:$rs2, simm12:$imm12, GPR:$rs1),
                "sw $rs2, ${imm12}(${rs1})",
                []>;
}

def : Pat<(store i32:$val, GPR:$base), (SW i32:$val, 0, GPR:$base)>;
def : Pat<(store i32:$val, frameindex:$base), (SW i32:$val, 0, GPR:$base)>;
def : Pat<(store i32:$val, tframeindex:$base), (SW i32:$val, 0, GPR:$base)>;


def LUI : U<OPC_LUI,
            (outs GPR:$rd),
            (ins i32imm:$imm20),
            "lui $rd, $imm20",
            []>;

def AUIPC : U<OPC_AUIPC,
            (outs GPR:$rd),
            (ins i32imm:$imm20),
            "auipc $rd, $imm20",
            []>;
let isBranch = true in {
    def BEQ : B<OPC_BEQ, "beq">;
    def BNE : B<OPC_BNE, "bne">;
    def BLT : B<OPC_BLT, "blt">;
    def BGE : B<OPC_BGE, "bge">;
    def BLTU : B<OPC_BLTU, "bltu">;
    def BGEU : B<OPC_BGEU, "bgeu">;
}

//def : Pat<(brcond (i32 (seteq GPR:$rs1, GPR:$rs2)), bb:$addr), (BEQ GPR:$rs1, GPR:$rs2, jmptarget:$addr)>;
//def : Pat<(brcond (i32 (setgt GPR:$rs1, GPR:$rs2)), bb:$addr), (BLT GPR:$rs2, GPR:$rs1, jmptarget:$addr)>;
//def : Pat<(brcond (i32 (setugt GPR:$rs1, GPR:$rs2)), bb:$addr), (BLTU GPR:$rs2, GPR:$rs1, jmptarget:$addr)>;

def : Pat<(brcc SETNE, GPR:$rs1, GPR:$rs2, bb:$addr), (BNE GPR:$rs1, GPR:$rs2, jmptarget:$addr)>;
def : Pat<(brcc SETEQ, GPR:$rs1, GPR:$rs2, bb:$addr), (BEQ GPR:$rs1, GPR:$rs2, jmptarget:$addr)>;


def hi : SDNode<"RISCVNISD::HI", SDTIntUnaryOp>;
def add_lo : SDNode<"RISCVNISD::ADD_LO", SDTIntBinOp>;

def : Pat<(hi tglobaladdr:$in), (LUI tglobaladdr:$in)>;
def : Pat<(add_lo GPR:$hi, tglobaladdr:$lo), (ADDI GPR:$hi, tglobaladdr:$lo)>;

//def FrameAddrRegImm : ComplexPattern<iPTR, 2, "SelectFrameAddrRegImm",
//    [frameindex, or, add]>;
//def : Pat<(FrameAddrRegImm (iPTR GPR:$rs1), i32imm:$imm12), (ADDI GPR:$rs1, imm:$imm12)>;


let isAsmParserOnly = 1 in
def PseudoLI : PseudoInst<
            (outs GPR:$rd),
            (ins i32imm:$imm12),
            [],
            "li $rd, $imm12">;         

let isBarrier = 1, isReturn = 1, isTerminator = 1 in {
  def PseudoRET : PseudoInst<
              (outs),
              (ins),
              [(retglue)]>,
              PseudoInstExpansion<(JALR X0, X1, 0)>;
}



def : InstAlias<"ret", (JALR X0, X1, 0)>;
def : InstAlias<"li $rd, $imm",  (ADDI GPR:$rd, X0, i32imm:$imm)>;
def : InstAlias<"j $offset", (JAL X0, jmptarget:$offset)>;