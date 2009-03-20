#ifndef _PLWIN32STATICSOUND_H
#define _PLWIN32STATICSOUND_H

#include "plWin32Sound.h"

DllClass plWin32StaticSound : public plWin32Sound {
public:
    DECLARE_CREATABLE(plWin32StaticSound)
};

DllClass plWin32GroupedSound : public plWin32StaticSound {
protected:
    hsTArray<unsigned int> fStartPositions;
    hsTArray<float> fVolumes;

public:
    DECLARE_CREATABLE(plWin32GroupedSound)

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);
    virtual void IRead(hsStream* S, plResManager* mgr);
    virtual void IWrite(hsStream* S, plResManager* mgr);

public:
    size_t getNumVolumes() const;
    unsigned int getPosition(size_t idx) const;
    float getVolume(size_t idx) const;
    void setVolumes(size_t count, unsigned int* positions, float* volumes);
};

DllClass plWin32LinkSound : public plWin32StaticSound {
public:
    DECLARE_CREATABLE(plWin32LinkSound)

    virtual void read(hsStream* S, plResManager* mgr);
};

#endif
