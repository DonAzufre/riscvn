//
// Created by DonAzufre on 25-5-5.
//

#ifndef LLVM_FTXT4KMCTARGETDESC_H
#define LLVM_FTXT4KMCTARGETDESC_H

#include "llvm/Config/config.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

#define GET_REGINFO_ENUM
#include "FTXT4KGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "FTXT4KGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "FTXT4KGenSubtargetInfo.inc"

#endif // LLVM_FTXT4KMCTARGETDESC_H
