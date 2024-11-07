class RISCVNReg<bits<5> Enc, string n, list<string> alt = []> : Register<n> {
  let HWEncoding{4-0} = Enc;
  let Namespace = "RISCVN";
  let AltNames = alt;
}

def ABIRegAltName : RegAltNameIndex;

let RegAltNameIndices = [ABIRegAltName] in {
  let isConstant = true in {
    def X0  : RISCVNReg<0, "x0", ["zero"]>;
  }
  def X1  : RISCVNReg<1,  "x1",  ["ra"]>;
  def X2  : RISCVNReg<2,  "x2",  ["sp"]>;
  def X3  : RISCVNReg<3,  "x3",  ["gp"]>;
  def X4  : RISCVNReg<4,  "x4",  ["tp"]>;
  def X5  : RISCVNReg<5,  "x5",  ["t0"]>;
  def X6  : RISCVNReg<6,  "x6",  ["t1"]>;
  def X7  : RISCVNReg<7,  "x7",  ["t2"]>;
  def X8  : RISCVNReg<8,  "x8",  ["s0", "fp"]>;
  def X9  : RISCVNReg<9,  "x9",  ["s1"]>;
  def X10 : RISCVNReg<10, "x10", ["a0"]>;
  def X11 : RISCVNReg<11, "x11", ["a1"]>;
  def X12 : RISCVNReg<12, "x12", ["a2"]>;
  def X13 : RISCVNReg<13, "x13", ["a3"]>;
  def X14 : RISCVNReg<14, "x14", ["a4"]>;
  def X15 : RISCVNReg<15, "x15", ["a5"]>;
  def X16 : RISCVNReg<16, "x16", ["a6"]>;
  def X17 : RISCVNReg<17, "x17", ["a7"]>;
  def X18 : RISCVNReg<18, "x18", ["s2"]>;
  def X19 : RISCVNReg<19, "x19", ["s3"]>;
  def X20 : RISCVNReg<20, "x20", ["s4"]>;
  def X21 : RISCVNReg<21, "x21", ["s5"]>;
  def X22 : RISCVNReg<22, "x22", ["s6"]>;
  def X23 : RISCVNReg<23, "x23", ["s7"]>;
  def X24 : RISCVNReg<24, "x24", ["s8"]>;
  def X25 : RISCVNReg<25, "x25", ["s9"]>;
  def X26 : RISCVNReg<26, "x26", ["s10"]>;
  def X27 : RISCVNReg<27, "x27", ["s11"]>;
  def X28 : RISCVNReg<28, "x28", ["t3"]>;
  def X29 : RISCVNReg<29, "x29", ["t4"]>;
  def X30 : RISCVNReg<30, "x30", ["t5"]>;
  def X31 : RISCVNReg<31, "x31", ["t6"]>;
}

def GPR : RegisterClass<"RISCVN", [i32], 32, (add 
    (sequence "X%u", 0, 31)
)>;