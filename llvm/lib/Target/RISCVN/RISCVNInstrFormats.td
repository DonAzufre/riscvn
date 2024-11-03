class RISCVNOpcode<string name, bits<7> val, bits<3> funct3> {
  string Name = name;
  bits<7> Value = val;
  bits<3> FUNCT3 = funct3;

}

def OPC_ADDI : RISCVNOpcode<"ADDI", 0b0010011, 0b000>;
def OPC_JALR : RISCVNOpcode<"JALR", 0b1101111, 0b000>;

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

class PseudoInst<dag outs, dag ins, list<dag> pattern, string asmstr = "">
    : RV32IInst<outs, ins, asmstr, pattern> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

class I<RISCVNOpcode opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : RV32IInst<outs, ins, asmstr, pattern> {
  bits<7> Opcode = opcode.Value;
  bits<5> rd;
  bits<3> funct3 = opcode.FUNCT3;
  bits<5> rs1;
  bits<12> imm12;

  let Inst{6-0} = Opcode;
  let Inst{11-7} = rd;
  let Inst{14-12} = funct3;
  let Inst{19-15} = rs1;
  let Inst{31-20} = imm12;
}