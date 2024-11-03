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
  def X1  : RISCVNReg<1, "x1", ["ra"]>;
  def X2  : RISCVNReg<2, "x2", ["sp"]>;
  def X10 : RISCVNReg<10,"x10", ["a0"]>;
  def X11 : RISCVNReg<11,"x11", ["a1"]>;
}

def GPR : RegisterClass<"RISCVN", [i32], 32, (add 
    X0, X1, X2, X10, X11
)>;