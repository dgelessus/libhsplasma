#ifndef _PLFACTORY_H
#define _PLFACTORY_H

#include "pdUnifiedTypeMap.h"

DllClass plFactory {
public:
    static class plCreatable* Create(short typeIdx);
    static class plCreatable* Create(short typeIdx, PlasmaVer ver);
    static class plCreatable* Create(const char* typeName);
    static const char* ClassName(short typeIdx);
    static const char* ClassName(short typeIdx, PlasmaVer ver);
    static short ClassIndex(const char* typeName);
    static short ClassVersion(short typeIdx, PlasmaVer ver);
};

#endif