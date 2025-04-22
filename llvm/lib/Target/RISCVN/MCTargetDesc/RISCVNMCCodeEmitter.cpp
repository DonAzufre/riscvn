#include "RISCVNMCTargetDesc.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace {

class RISCVNMCCodeEmitter : public MCCodeEmitter {
  RISCVNMCCodeEmitter(const RISCVNMCCodeEmitter &) = delete;
  RISCVNMCCodeEmitter &operator=(const RISCVNMCCodeEmitter &) = delete;
  MCContext &Ctx;

public:
  RISCVNMCCodeEmitter(const MCInstrInfo &, MCContext &Ctx) : Ctx(Ctx) {}

  ~RISCVNMCCodeEmitter() override = default;

  void encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  unsigned long long getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const;
  unsigned long long
  getPCRelBranchTargetOpValue(const MCInst &MI, unsigned OpNo,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &STI) const;
  unsigned getBranchTargetOpValue(const MCInst &MI, unsigned OpNo,
                                  SmallVectorImpl<MCFixup> &Fixups,
                                  const MCSubtargetInfo &STI) const;
  unsigned getCallTargetOpValue(const MCInst &MI, unsigned OpNo,
                                SmallVectorImpl<MCFixup> &Fixups,
                                const MCSubtargetInfo &STI) const;
};

} // end anonymous namespace

void RISCVNMCCodeEmitter::encodeInstruction(const MCInst &MI,
                                            SmallVectorImpl<char> &CB,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  // todo: 在这里展开伪指令
  uint32_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  support::endian::write<uint32_t>(CB, Bits, llvm::endianness::little);
}

unsigned long long
RISCVNMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return (unsigned long long)Ctx.getRegisterInfo()->getEncodingValue(
        MO.getReg());

  if (MO.isImm()) {
    return static_cast<unsigned long long>(MO.getImm());
  }

  llvm_unreachable("Unhandled expression!");
  return 0;
}

#include "RISCVNGenMCCodeEmitter.inc"

MCCodeEmitter *llvm::createRISCVNMCCodeEmitter(const MCInstrInfo &MCII,
                                               MCContext &Ctx) {
  return new RISCVNMCCodeEmitter(MCII, Ctx);
}

unsigned long long RISCVNMCCodeEmitter::getPCRelBranchTargetOpValue(
    const MCInst &MI, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  if (MO.isReg() || MO.isImm())
    return getMachineOpValue(MI, MO, Fixups, STI);
  return 0;
}

unsigned
RISCVNMCCodeEmitter::getCallTargetOpValue(const MCInst &MI, unsigned OpNo,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  return 0;
}
