#ifndef LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNMCTARGETDESC_H
#define LLVM_LIB_TARGET_RISCVN_MCTARGETDESC_RISCVNMCTARGETDESC_H

#include "llvm/Config/config.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class Target;

MCCodeEmitter *createRISCVNMCCodeEmitter(const MCInstrInfo &MCII,
                                         MCContext &Ctx);

MCAsmBackend *createRISCVNAsmBackend(const Target &T,
                                     const MCSubtargetInfo &STI,
                                     const MCRegisterInfo &MRI,
                                     const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter>
createRISCVNELFObjectWriter(bool Is64Bit, uint8_t OSABI);
} // namespace llvm

#define GET_REGINFO_ENUM
#include "RISCVNGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "RISCVNGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "RISCVNGenSubtargetInfo.inc"

#endif
