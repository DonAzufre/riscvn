//
// Created by myrica on 4/16/25.
//

#include "RISCVNMCExpr.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbolELF.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
const llvm::RISCVNMCExpr *llvm::RISCVNMCExpr::create(const MCExpr *Expr,
                                                     VariantKind Kind,
                                                     MCContext &Ctx) {
  return new (Ctx) RISCVNMCExpr(Expr, Kind);
}
void llvm::RISCVNMCExpr::printImpl(raw_ostream &OS,
                                   const MCAsmInfo *MAI) const {
  auto Kind = getKind();
  bool HasVariant = (Kind != VK_RISCVN_None);
  if (HasVariant)
    OS << '%' << getVariantKindName(getKind()) << '(';

  Expr->print(OS, MAI);
  if (HasVariant)
    OS << ')';
}
bool llvm::RISCVNMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                                   const MCAssembler *Asm,
                                                   const MCFixup *Fixup) const {
  if (!getSubExpr()->evaluateAsRelocatable(Res, nullptr, nullptr))
    return false;

  Res =
      MCValue::get(Res.getSymA(), Res.getSymB(), Res.getConstant(), getKind());
  return Res.getSymB() ? getKind() == VK_RISCVN_None : true;
}
void llvm::RISCVNMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}
llvm::MCFragment *llvm::RISCVNMCExpr::findAssociatedFragment() const {
  return getSubExpr()->findAssociatedFragment();
}
void llvm::RISCVNMCExpr::fixELFSymbolsInTLSFixups(MCAssembler &) const {
  llvm_unreachable(__PRETTY_FUNCTION__);
}

llvm::StringRef llvm::RISCVNMCExpr::getVariantKindName(VariantKind Kind) {
  switch (Kind) {
  default:
    llvm_unreachable("invalid vk kind");

  case VK_RISCVN_HI:
    return "hi";
  case VK_RISCVN_LO:
    return "lo";
  }
}
