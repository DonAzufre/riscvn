class FTXT4KReg<string name> : Register<name> {
    let Namespace = "FTXT4K";
}

class AddrReg<bits<5> Enc, string n> : FTXT4KReg<n> {
    let HWEncoding{4-0} = Enc;
}

class ScalarReg<bits<6> Enc, string n> : FTXT4KReg<n> {
    let HWEncoding{5-0} = Enc;
}

foreach I = 0-63 in {
    def R#I : ScalarReg<I, "R"#I>;
}

foreach I = 0-15 in {
    def AR#I : AddrReg<I, "AR"#I>;
}

def GPR : RegisterClass<"FTXT4K", [i64], 64,
    (add (sequence "R%u", 0, 63))>;

// scalar conditional reg
def SCD : RegisterClass<"FTXT4K", [i64], 64,
    (sequence "R%u", 0, 6)>;


// scalar addr reg
def SAR : RegisterClass<"FTXT4K", [i64], 64,
    (sequence "AR%u", 8, 15)>;

def SAR_40LS : RegisterClass<"FTXT4K", [i64], 64,
    (sequence "AR%u", 12, 15)>;