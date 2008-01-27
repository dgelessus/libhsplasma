#include "plSoftVolume.h"

// plSoftVolume //
plSoftVolume::plSoftVolume() : fListenStrength(0.0f), fListenState(kListenNone),
                               fInsideStrength(0.0f), fOutsideStrength(0.0f) { }

plSoftVolume::~plSoftVolume() { }

IMPLEMENT_CREATABLE(plSoftVolume, kSoftVolume, plRegionBase)

void plSoftVolume::read(hsStream* S, plResManager* mgr) {
    plObjInterface::read(S, mgr);

    fListenState = S->readInt();
    fInsideStrength = S->readFloat();
    fOutsideStrength = S->readFloat();
}

void plSoftVolume::write(hsStream* S, plResManager* mgr) {
    plObjInterface::write(S, mgr);

    S->writeInt(fListenState);
    S->writeFloat(fInsideStrength);
    S->writeFloat(fOutsideStrength);
}

void plSoftVolume::prcWrite(pfPrcHelper* prc) {
    plObjInterface::prcWrite(prc);

    prc->startTag("SoftVolumeParams");
    prc->writeParam("ListenState", fListenState);
    prc->writeParam("InsideStrength", fInsideStrength);
    prc->writeParam("OutsideStrength", fOutsideStrength);
    prc->endTag(true);
}


// plSoftVolumeSimple //
plSoftVolumeSimple::plSoftVolumeSimple() : fVolume(NULL), fSoftDist(0.0f) { }

plSoftVolumeSimple::~plSoftVolumeSimple() {
    if (fVolume != NULL)
        delete fVolume;
}

IMPLEMENT_CREATABLE(plSoftVolumeSimple, kSoftVolumeSimple, plSoftVolume)

void plSoftVolumeSimple::read(hsStream* S, plResManager* mgr) {
    plSoftVolume::read(S, mgr);

    fSoftDist = S->readFloat();
    fVolume = plVolumeIsect::Convert(mgr->ReadCreatable(S));
}

void plSoftVolumeSimple::write(hsStream* S, plResManager* mgr) {
    plSoftVolume::write(S, mgr);

    S->writeFloat(fSoftDist);
    mgr->WriteCreatable(S, fVolume);
}

void plSoftVolumeSimple::prcWrite(pfPrcHelper* prc) {
    plSoftVolume::prcWrite(prc);

    prc->startTag("Volume");
    prc->writeParam("SoftDist", fSoftDist);
    prc->endTag();
    fVolume->prcWrite(prc); prc->closeTag();
    prc->closeTag();
}


// plSoftVolumeComplex //
plSoftVolumeComplex::plSoftVolumeComplex() { }
plSoftVolumeComplex::~plSoftVolumeComplex() { }

IMPLEMENT_CREATABLE(plSoftVolumeComplex, kSoftVolumeComplex, plSoftVolume)

void plSoftVolumeComplex::read(hsStream* S, plResManager* mgr) {
    plSoftVolume::read(S, mgr);

    fSubVolumes.setSize(S->readInt());
    for (size_t i=0; i<fSubVolumes.getSize(); i++)
        fSubVolumes[i] = mgr->readKey(S);
}

void plSoftVolumeComplex::write(hsStream* S, plResManager* mgr) {
    plSoftVolume::write(S, mgr);

    S->writeInt(fSubVolumes.getSize());
    for (size_t i=0; i<fSubVolumes.getSize(); i++)
        mgr->writeKey(S, fSubVolumes[i]);
}

void plSoftVolumeComplex::prcWrite(pfPrcHelper* prc) {
    plSoftVolume::prcWrite(prc);

    prc->writeSimpleTag("SubVolumes");
    for (size_t i=0; i<fSubVolumes.getSize(); i++)
        fSubVolumes[i]->prcWrite(prc);
    prc->closeTag();
}


// plSoftVolumeIntersect //
plSoftVolumeIntersect::plSoftVolumeIntersect() { }
plSoftVolumeIntersect::~plSoftVolumeIntersect() { }

IMPLEMENT_CREATABLE(plSoftVolumeIntersect, kSoftVolumeIntersect,
                    plSoftVolumeComplex)


// plSoftVolumeInvert //
plSoftVolumeInvert::plSoftVolumeInvert() { }
plSoftVolumeInvert::~plSoftVolumeInvert() { }

IMPLEMENT_CREATABLE(plSoftVolumeInvert, kSoftVolumeInvert, plSoftVolumeComplex)


// plSoftVolumeUnion //
plSoftVolumeUnion::plSoftVolumeUnion() { }
plSoftVolumeUnion::~plSoftVolumeUnion() { }

IMPLEMENT_CREATABLE(plSoftVolumeUnion, kSoftVolumeUnion, plSoftVolumeComplex)
