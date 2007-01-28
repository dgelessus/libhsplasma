#include "plGeometrySpan.h"

plGeometrySpan::plGeometrySpan(PlasmaVer pv) { }

void plGeometrySpan::Read(hsStream *S) {
	ClearBuffers();
    fLocalToWorld.read(S);
    fWorldToLocal.read(S);
    fLocalBounds.read(S);
    //if (fWorldBounds != fLocalBounds)
    //    fWorldBounds.Reset(fLocalBounds);
    //fWorldBounds.Transform(&fLocalToWorld);
    fOBBToLocal.read(S);
    fLocalToOBB.read(S);
    
    fBaseMatrix = S->readInt();
    fNumMatrices = S->readByte();
    fLocalUVWChans = S->readShort();
    fMaxBoneIdx = S->readShort();
    fPenBoneIdx = S->readShort();
    fMinDist = S->readFloat();
    fMaxDist = S->readFloat();
    fFormat = S->readByte();
    fProps = S->readInt();
    fNumVerts = S->readInt();
    fNumIndices = S->readInt();
    S->readInt();  // Discarded
    S->readByte(); // Discarded
    fDecalLevel = S->readInt();
    
    if (fProps & kWaterHeight)
        fWaterHeight = S->readFloat();
    
    if (fNumVerts > 0) {
        // this is hsPoint3[UVCount] + fPosition + fNormal.
        unsigned int size = ((fFormat & kUVCountMask) + 2) * sizeof(hsPoint3);
        if (fFormat & kSkin3Weights == kSkin1Weight)
            size += 4;  // 1 float
        else if (fFormat & kSkin3Weights == kSkin2Weights)
            size += 8;  // 2 floats
        else if (fFormat & kSkin3Weights == kSkin3Weights)
            size += 12; // 3 floats
        if (fFormat & kSkinIndices)
            size += 4;  // uint32
        fVertexData = malloc(fNumVerts * size);
        S->read(fNumVerts * size, fVertexData);
        
        fMultColor = new hsColorRGBA[fNumVerts];
        fAddColor = new hsColorRGBA[fNumVerts];
        for (int i=0; i<fNumVerts; i++) {
            fMultColor[i].r = S->readFloat();
            fMultColor[i].g = S->readFloat();
            fMultColor[i].b = S->readFloat();
            fMultColor[i].a = S->readFloat();
            fAddColor[i].r = S->readFloat();
            fAddColor[i].g = S->readFloat();
            fAddColor[i].b = S->readFloat();
            fAddColor[i].a = S->readFloat();
        }
        fDiffuseRGBA = new unsigned int[fNumVerts];
        fSpecularRGBA = new unsigned int[fNumVerts];
        stream->readInts(fNumVerts, fDiffuseRGBA);
        stream->readInts(fNumVerts, fSpecularRGBA);
    } else {
        fVertexData = NULL;
        fMultColor = NULL;
        fAddColor = NULL;
        fDiffuseRGBA = NULL;
        fSpecularRGBA = NULL;
    }
    if (fNumIndices > 0) {
        fIndexData = new unsigned short[fNumVerts];
        stream->readShorts(fNumVerts, fIndexData);
    } else {
        fIndexData = NULL;
    }
    fInstanceGroup = S->readInt();
    if (fInstanceGroup != 0) {
        fInstanceRefs = IGetInstanceGroup(fInstanceGroup, stream->readInt());
        fInstanceRefs->append(this);
    }
}

void plGeometrySpan::IClearMembers() {
    fVertexData = NULL;
    fIndexData = NULL;
    fMaterial = NULL;
    fNumIndices = 0;
    fNumVerts = 0;
    fNumMatrices = 0;
    fBaseMatrix = 0;
    fLocalUVWChans = 0;
    fMaxBoneIdx = 0;
    fPenBoneIdx = 0;
    fCreating = false;
    fFogEnviron = NULL;
    fProps = 0;
    fWaterHeight = 0.0;
    fMultColor = NULL;
    fAddColor = NULL;
    fDiffuseRGBA = NULL;
    fSpecularRGBA = NULL;
    fInstanceRefs = NULL;
    fInstanceGroup = 0;
    fSpanRefIndex = 0xFFFFFFFF;
    fMaxDist = -1.0;
    fMinDist = -1.0;
	fLocalToOBB.Identity();
    fOBBToLocal.Identity();
    fDecalLevel = 0;
    fMaxOwner = NULL;
}

void plGeometrySpan::ClearBuffers() {
    if (fProps & kUserOwned) {
        IClearMembers();
        return;
    }
    if (fInstanceRefs != NULL) {
        if (fInstanceRefs->getSize() != 1) {
            delete fInstanceRefs;
            IClearGroupID(fInstanceGroupID);
        } else {
            fInstanceRefs->remove(fInstanceRefs->Find(this));
        }
        fInstanceRefs = NULL;
        fInstanceGroupID = 0;
    } else {
        delete[] fVertexData;
        fVertexData = NULL;
        delete[] fMultColor;
        delete[] fAddColor;
        fMultColor = NULL;
        fAddColor = NULL;
    }
    delete[] fIndexData;
    delete[] fDiffuseRGBA;
    delete[] fSpecularRGBA;
    fIndexData = NULL;
    fDiffuseRGBA = NULL;
    fSpecularRGBA = NULL;
}
