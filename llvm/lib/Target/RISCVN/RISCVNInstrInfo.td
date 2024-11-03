include "RISCVNInstrFormats.td"


def retglue : SDNode<"RISCVNISD::RET_GLUE", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;


def ADDI : I<OPC_ADDI,
            (outs GPR:$rd),
            (ins GPR:$rs1, i32imm:$imm12),
            "addi $rd, $rs1, $imm12",
            []>;

def : Pat<(i32 (imm:$imm12)), (ADDI X0, imm:$imm12)>;          

let isCall = 1 in {
  def JALR : I<OPC_JALR,
              (outs GPR:$rd),
              (ins GPR:$rs1, i32imm:$imm12),
              "jalr $rd, $rs1, $imm12",
              []>;
}                        

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