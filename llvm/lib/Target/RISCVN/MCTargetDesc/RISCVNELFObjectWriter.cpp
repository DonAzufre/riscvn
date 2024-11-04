#include "MCTargetDesc/RISCVNMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"

using namespace llvm;

namespace {
class RISCVNELFObjectWriter : public MCELFObjectTargetWriter {
public:
  RISCVNELFObjectWriter(bool Is64Bit, uint8_t OSABI)
      : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_RISCV,
                                /*HasRelocationAddend*/ true) {}

  ~RISCVNELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
} // namespace

unsigned RISCVNELFObjectWriter::getRelocType(MCContext &Ctx,
                                             const MCValue &Target,
                                             const MCFixup &Fixup,
                                             bool IsPCRel) const {
  return ELF::R_RISCVN_NONE;
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createRISCVNELFObjectWriter(bool Is64Bit, uint8_t OSABI) {
  return std::make_unique<RISCVNELFObjectWriter>(Is64Bit, OSABI);
}