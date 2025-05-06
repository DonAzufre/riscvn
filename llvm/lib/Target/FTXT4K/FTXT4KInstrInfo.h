//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KINSTRINFO_H
#define FTXT4KINSTRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"


#define GET_INSTRINFO_HEADER
#include "FTXT4KGenInstrInfo.inc"

namespace llvm {
    class FTXT4KSubtarget;

    class FTXT4KInstrInfo : public  FTXT4KGenInstrInfo {
        const FTXT4KSubtarget& STI;
    public:
        explicit FTXT4KInstrInfo(FTXT4KSubtarget& STI);
    };

}

#endif //FTXT4KINSTRINFO_H
