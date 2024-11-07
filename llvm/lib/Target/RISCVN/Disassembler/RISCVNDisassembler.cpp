//===-- RISCVNDisassembler.cpp - Disassembler for RISCVN ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the RISCVNDisassembler class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "TargetInfo/RISCVNTargetInfo.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Endian.h"

using namespace llvm;

#define DEBUG_TYPE "RISCVN-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

namespace {
class RISCVNDisassembler : public MCDisassembler {
public:
  RISCVNDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}

  DecodeStatus getInstruction(MCInst &Instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};
} // end namespace

static MCDisassembler *createRISCVNDisassembler(const Target &T,
                                                const MCSubtargetInfo &STI,
                                                MCContext &Ctx) {
  return new RISCVNDisassembler(STI, Ctx);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCVNDisassembler() {
  // Register the disassembler for each target.
  TargetRegistry::RegisterMCDisassembler(getTheRISCVNTarget(),
                                         createRISCVNDisassembler);
}

static DecodeStatus DecodeGPRRegisterClass(MCInst &Inst, uint64_t RegNo,
                                           uint64_t Address,
                                           const MCDisassembler *Decoder) {
  if (RegNo >= 32)
    return MCDisassembler::Fail;
  Inst.addOperand(MCOperand::createReg(RISCVN::X0 + RegNo));
  return MCDisassembler::Success;
}

#include "RISCVNGenDisassemblerTables.inc"

DecodeStatus RISCVNDisassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                                ArrayRef<uint8_t> Bytes,
                                                uint64_t Address,
                                                raw_ostream &CS) const {
  uint32_t Insn;
  DecodeStatus Result;

  // We want to read exactly 4 bytes of data because all RISCVN instructions
  // are fixed 32 bits.
  if (Bytes.size() < 4) {
    Size = 0;
    return MCDisassembler::Fail;
  }

  Insn = support::endian::read32le(Bytes.data());
  // Calling the auto-generated decoder function.
  Result = decodeInstruction(DecoderTable32, MI, Insn, Address, this, STI);

  if (Result != MCDisassembler::Fail) {
    Size = 4;
    return Result;
  }

  return MCDisassembler::Fail;
} 