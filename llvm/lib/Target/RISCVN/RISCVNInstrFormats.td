class RISCVNBaseOpcode<string name, bits<7> opcode> {
    string Name = name;
    bits<7> Value = opcode;
}

class ITypeOpcode<string name, bits<7> val, bits<3> funct3>
    : RISCVNBaseOpcode<name, val>{
  bits<3> FUNCT3 = funct3;
}

class STypeOpcode<string name, bits<3> funct3>
    : ITypeOpcode<name, 0b0100011, funct3> {
}

class BTypeOpcode<string name, bits<3> funct3>
    : ITypeOpcode<name, 0b1100011, funct3>;

class RTypeOpcode<string name, bits<3> funct3, bits<7> funct7>
    : ITypeOpcode<name, 0b0110011, funct3>{
  bits<7> FUNCT7 = funct7;
}

class JTypeOpcode<string name>
    : RISCVNBaseOpcode<name, 0b1101111> {
}

class UTypeOpcode<string name, bits<7> opcode>
    : RISCVNBaseOpcode<name, opcode>;

// I type inst
def OPC_ADDI : ITypeOpcode<"ADDI", 0b0010011, 0b000>;
def OPC_JALR : ITypeOpcode<"JALR", 0b1100111, 0b000>;
def OPC_LW : ITypeOpcode<"LW", 0b0000011, 0b010>;

// R type inst
def OPC_ADD : RTypeOpcode<"ADD", 0b000, 0b0000000>;
def OPC_SUB : RTypeOpcode<"SUB", 0b000, 0b0100000>;
def OPC_SLL : RTypeOpcode<"SLL", 0b001, 0b0000000>;
def OPC_SLT : RTypeOpcode<"SLT", 0b010, 0b0000000>;
def OPC_SLTU : RTypeOpcode<"SLTU", 0b011, 0b0000000>;
def OPC_XOR : RTypeOpcode<"XOR", 0b100, 0b0000000>;
def OPC_SRL : RTypeOpcode<"SRL", 0b101, 0b0000000>;
def OPC_SRA : RTypeOpcode<"SRA", 0b010, 0b0100000>;
def OPC_OR : RTypeOpcode<"OR", 0b110, 0b0000000>;
def OPC_AND : RTypeOpcode<"AND", 0b111, 0b0000000>;

// S type inst
def OPC_SW : STypeOpcode<"SW", 0b010>;

// J type inst
def OPC_JAL :JTypeOpcode<"JAL">;

// U type inst
def OPC_LUI : UTypeOpcode<"LUI", 0b0110111>;
def OPC_AUIPC : UTypeOpcode<"AUIPC", 0b0010111>;

// B type inst
def OPC_BEQ : BTypeOpcode<"BEQ", 0b000>;
def OPC_BNE : BTypeOpcode<"BNE", 0b001>;
def OPC_BLT : BTypeOpcode<"BLT", 0b100>;
def OPC_BGE : BTypeOpcode<"BGE", 0b101>;
def OPC_BLTU : BTypeOpcode<"BLTU", 0b110>;
def OPC_BGEU : BTypeOpcode<"BGEU", 0b111>;

class RV32IInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;
  field bits<32> SoftFail = 0;

  let Size = 4;

  let Namespace = "RISCVN";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

//class RV32RInst<dag outs, dag ins, string asmstr, list<dag> pattern>
//    : Instructon {
//
//}

class PseudoInst<dag outs, dag ins, list<dag> pattern, string asmstr = "">
    : RV32IInst<outs, ins, asmstr, pattern> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

class I<ITypeOpcode opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : RV32IInst<outs, ins, asmstr, pattern> {
  bits<7> Opcode = opcode.Value;
  bits<5> rd;
  bits<3> funct3 = opcode.FUNCT3;
  bits<5> rs1;
  bits<12> imm12 = 0;

  let Inst{6-0} = Opcode;
  let Inst{11-7} = rd;
  let Inst{14-12} = funct3;
  let Inst{19-15} = rs1;
  let Inst{31-20} = imm12;
}

class R<RTypeOpcode opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : RV32IInst<outs, ins, asmstr, pattern> {
  bits<7> Opcode = opcode.Value;
  bits<5> rd;
  bits<3> funct3 = opcode.FUNCT3;
  bits<5> rs1;
  bits<5> rs2;
  bits<7> funct7 = opcode.FUNCT7;

  let Inst{6-0} = Opcode;
  let Inst{11-7} = rd;
  let Inst{14-12} = funct3;
  let Inst{19-15} = rs1;
  let Inst{24-20} = rs2;
  let Inst{31-25} = funct7;
}

class J<JTypeOpcode opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : RV32IInst<outs, ins, asmstr, pattern> {
  bits<7> Opcode = opcode.Value;
  bits<5> rd;
  bits<20> imm20;

  let Inst{6-0} = Opcode;
  let Inst{11-7} = rd;
  let Inst{31-12} = imm20;
}

class S<STypeOpcode opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    :RV32IInst<outs, ins, asmstr, pattern>  {
  bits<7> Opcode = opcode.Value;
  bits<3> funct3 = opcode.FUNCT3;
  bits<5> rs1;
  bits<5> rs2;
  bits<12> imm12;

  let Inst{6-0} = Opcode;
  let Inst{11-7} = imm12{4-0};
  let Inst{14-12} = funct3;
  let Inst{19-15} = rs1;
  let Inst{24-20} = rs2;
  let Inst{31-25} = imm12{11-5};
}

def jmptarget : Operand<OtherVT> {
  let EncoderMethod = "getPCRelBranchTargetOpValue";
  let OperandType = "OPERAND_PCREL";
  let PrintMethod = "printJmpTargetOperand";
}

class U<UTypeOpcode opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    :RV32IInst<outs, ins, asmstr, pattern> {
  bits<7> Opcode = opcode.Value;
  bits<5> rd;
  bits<20> imm20;

  let Inst{6-0} = Opcode;
  let Inst{11-7} = rd;
  let Inst{31-12} = imm20;
}

class B<BTypeOpcode opcode, string instname, list<dag> pattern = []>
    :RV32IInst<(outs ), (ins GPR:$rs1, GPR:$rs2, jmptarget:$imm12), instname#" $rs1, $rs2, $imm12", pattern>  {
  bits<7> Opcode = opcode.Value;
  bits<3> funct3 = opcode.FUNCT3;
  bits<5> rs1;
  bits<5> rs2;
  bits<12> imm12;

  let Inst{6-0} = Opcode;
  let Inst{11-7} = imm12{4-0};
  let Inst{14-12} = funct3;
  let Inst{19-15} = rs1;
  let Inst{24-20} = rs2;
  let Inst{31-25} = imm12{11-5};
}
