#ifndef _PLLEAFCONTROLLER_H
#define _PLLEAFCONTROLLER_H

#include "plController.h"

DllClass plLeafController : public plController {
protected:
    unsigned int fType;
    hsTArray<hsKeyFrame*> fKeys;
    hsTArray<class plEaseController*> fControllers;
    unsigned int fLastKeyIdx;

public:
    plLeafController();
    virtual ~plLeafController();

    DECLARE_CREATABLE(plLeafController)

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);
    virtual void prcWrite(pfPrcHelper* prc);

protected:
    void AllocKeys(unsigned int numKeys, unsigned char type);
    void DeallocKeys();
    void AllocControllers(unsigned int numControllers);
    void DeallocControllers();
};



#endif
