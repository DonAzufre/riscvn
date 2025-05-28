//
// Created by DonAzufre on 25-5-5.
//

#ifndef FTXT4KMCASMINFO_H
#define FTXT4KMCASMINFO_H


#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

    class Triple;

    class FTXT4KMCAsmInfo : public MCAsmInfoELF {
    public:
        bool shouldOmitSectionDirective(StringRef SectionName) const override;

        explicit FTXT4KMCAsmInfo(const Triple &TargetTriple);
    };
} // namespace llvm

#endif //FTXT4KMCASMINFO_H
