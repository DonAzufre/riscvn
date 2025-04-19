//
// Created by myrica on 4/16/25.
//

#ifndef LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNMCEXPR_H
#define LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNMCEXPR_H

#include "llvm/MC/MCExpr.h"

namespace llvm {

class RISCVNMCExpr : public MCTargetExpr {
public:
  enum VariantKind {
    VK_RISCVN_None,
    VK_RISCVN_LO,
    VK_RISCVN_HI,
    VK_RISCVN_Invalid,
  };

private:
  const MCExpr *Expr;
  const VariantKind Kind;

  explicit RISCVNMCExpr(const MCExpr *Expr, VariantKind Kind)
      : Expr(Expr), Kind(Kind) {}

public:
  static const RISCVNMCExpr *create(const MCExpr *Expr, VariantKind Kind,
                                    MCContext &Ctx);

  VariantKind getKind() const { return Kind; }
  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAssembler *Asm,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override;
  void fixELFSymbolsInTLSFixups(MCAssembler &) const override;

  const MCExpr *getSubExpr() const { return Expr; }
  static bool classof(const MCExpr *Expr) {
    return Expr->getKind() == MCExpr::Target;
  }
  static StringRef getVariantKindName(VariantKind Kind);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNMCEXPR_H
