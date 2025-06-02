//
// Created by DonAzufre on 25-5-5.
//

#include "FTXT4KMCAsmInfo.h"


using namespace llvm;

bool FTXT4KMCAsmInfo::shouldOmitSectionDirective(StringRef SectionName) const {
    return false;
}

FTXT4KMCAsmInfo::FTXT4KMCAsmInfo(const Triple &TT) {
    CommentString = ";";
    LinkerPrivateGlobalPrefix = "$";
    PrivateGlobalPrefix= "$";
    PrivateLabelPrefix = "LBB";
    UsesELFSectionDirectiveForBSS = false;
    GlobalDirective = "\t.global\t";
    HasDotTypeDotSizeDirective = false;
    HasSingleParameterDotFile = false;
}
