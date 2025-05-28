class FTXT4KInst<dag outs, dag ins, string asm, list<dag> pattern>
    : Instruction {
    field bits<40> Inst;
    field bits<40> SoftFail;

    let Size = 5;
    let Namespace = "FTXT4K";

    dag OutOperandList = outs;
    dag InOperandList = ins;
    let AsmString = asm;
    let Pattern = pattern;
}

class Ins40Pseudo<dag outs, dag ins, string asm, list<dag> pattern>
    : FTXT4KInst<outs, ins, asm, pattern> {
    let isPseudo = 1;
    let isCodeGenOnly = 1;
}

class FTXT4KInst80<dag outs, dag ins, string asm, list<dag> pattern>
    : Instruction {
    field bits<80> Inst;
    field bits<80> SoftFail;

    let Size = 10;
    let Namespace = "FTXT4K";

    dag OutOperandList = outs;
    dag InOperandList = ins;
    let AsmString = asm;
    let Pattern = pattern;
}

class Ins40Base<dag outs, dag ins, string asm, list<dag> pattern>
    : FTXT4KInst<outs, ins, asm, pattern> {

    // 0 = scaler, 1 = vector
    bit V = 0;
    // 0 = no parallel
    bit P = 0;
    // 0 = if (cond) inst, 1 = if(!cond) inst
    bit Z = 1;
    // cond reg
    bits<3> CondReg = 0b000;

    let Inst{0} = P;
    let Inst{1} = V;
    let Inst{36} = Z;
    let Inst{39-37} = CondReg;
}

class Ins80Base<dag outs, dag ins, string asm, list<dag> pattern>
    : FTXT4KInst80<outs, ins, asm, pattern> {

    // 0 = scaler, 1 = vector
    bit V = 0;
    // 0 = no parallel
    bit P = 0;
    // 0 = if (cond) inst, 1 = if(!cond) inst
    bit Z = 1;
    // cond reg
    bits<3> CondReg = 0b000;

    let Inst{0} = P;
    let Inst{1} = V;
    let Inst{36} = Z;
    let Inst{39-37} = CondReg;
}

class Ins40Mov<dag outs, dag ins, string asm, list<dag> pattern>
    : Ins40Base<outs, ins, asm, pattern> {

    bits<24> imm24;
    bits<6> dst;
    // function unit
    bits<2> U = 0b00;

    let Inst{35-30} = dst;
    let Inst{29-6} = imm24;
    let Inst{5-4} = U;
    let Inst{3-2} = 0b00;
}

class Ins80Mov<bits<2> unit, dag outs, dag ins, string asm, list<dag> pattern>
    : Ins80Base<outs, ins, asm, pattern> {

    bits<64> imm64;
    bits<6> dst;
    // function unit
    bits<2> U = unit;

    let Inst{79-40} = imm64{63-24};
    let Inst{35-30} = dst;
    let Inst{29-6} = imm64{23-0};
    let Inst{5-4} = U;
    let Inst{3-2} = 0b01;
}

class Ins40Triple<dag outs, dag ins, string asm, list<dag> pattern>
    : Ins40Base<outs, ins, asm, pattern> {

    bits<6> dst;
    bits<6> src1;
    bits<6> src2;
    bits<6> src3;
    bits<5> op;
    // function unit
    bits<2> U = 0b00;

    let Inst{35-30} = dst;
    let Inst{29-24} = src1;
    let Inst{23-18} = src2;
    let Inst{17-12} = src3;
    let Inst{11-7} = op;
    let Inst{6-5} = U;
    let Inst{4-2} = 0b011;
}

class Ins40Br<dag outs, dag ins, string asm, list<dag> pattern>
    : Ins40Base<outs, ins, asm, pattern> {

    bits<29> imm29;
    bits<2> op;

    let Inst{35-7} = imm29;
    let Inst{6-5} = op;
    let Inst{4-2} = 0b110;
}

class Ins40LS<bits<4> InsType, dag outs, dag ins, string asm, list<dag> pattern>
    : Ins40Base<outs, ins, asm, pattern> {

    bits<18> ucst18;
    bits<6> dst;
    bits<4> ar;
    bit Reserved = 0b0;

    let Inst{2-4} = 0b010;
    let Inst{5} = Reserved;
    let Inst{6-7} = ar{2-3};
    let Inst{8-11} = InsType;
    let Inst{12-29} = ucst18;
    let Inst{30-35} = dst;
}

// ins 40bit with Single or Double operands
class Ins40SD<bits<10> op, bits<3> unit, dag outs, dag ins, string asm, list<dag> pattern>
    : Ins40Base<outs, ins, asm, pattern> {

    bits<6> src1 = 0b000000;
    bits<6> src2 = 0b000000;
    bits<6> dst = 0b000000;
    bits<3> U = unit;

    let Inst{2-4} = 0b111;
    let Inst{5-7} = U;
    let Inst{8-17} = op;
    let Inst{18-23} = src2;
    let Inst{24-29} = src1;
    let Inst{30-25} = dst;
}