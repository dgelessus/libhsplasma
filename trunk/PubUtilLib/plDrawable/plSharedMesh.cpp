#include "plSharedMesh.h"
#include "../plResMgr/plResManager.h"

plSharedMesh::plSharedMesh(PlasmaVer pv) : fFlags(1) { }

short plSharedMesh::ClassIndex() {
    return (ver == pvEoa ? -1 : 0x00CC);
}
