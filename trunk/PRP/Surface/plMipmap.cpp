#include "plMipmap.h"
#include <cstring>
#include <cstdlib>

plMipmap::plMipmap()
        : fImageData(NULL), fJPEGData(NULL), fJPEGSize(0), fAlphaData(NULL),
          fAlphaSize(0), fJPEGDataRLE(NULL), fAlphaDataRLE(NULL),
          fTotalSize(0), fNumLevels(0), fLevelSizes(NULL), fCurLevelPtr(NULL) {
    fPixelSize = 32;
    fSpace = kDirectSpace;
    fFlags = kAlphaChannelFlag;
}

plMipmap::plMipmap(unsigned int width, unsigned int height, unsigned int cfg,
                   unsigned char numLevels, unsigned char compType,
                   unsigned char format)
        : fImageData(NULL), fJPEGData(NULL), fJPEGSize(0), fAlphaData(NULL),
          fAlphaSize(0), fJPEGDataRLE(NULL), fAlphaDataRLE(NULL),
          fTotalSize(0), fNumLevels(0), fLevelSizes(NULL), fCurLevelPtr(NULL) {
    fPixelSize = 32;
    fSpace = kDirectSpace;
    fFlags = kAlphaChannelFlag;
    Create(width, height, cfg, numLevels, compType, format);
}

plMipmap::~plMipmap() {
    if (fImageData != NULL) delete[] fImageData;
    if (fJPEGData != NULL) delete[] fJPEGData;
    if (fAlphaData != NULL) delete[] fAlphaData;
    if (fLevelSizes != NULL) delete[] fLevelSizes;
    if (fJPEGDataRLE != NULL) delete fJPEGDataRLE;
    if (fAlphaDataRLE != NULL) delete fAlphaDataRLE;
}

IMPLEMENT_CREATABLE(plMipmap, kMipmap, plBitmap)

void plMipmap::Create(unsigned int width, unsigned int height, unsigned int cfg,
                      unsigned char numLevels, unsigned char compType,
                      unsigned char format) {
    setConfig(cfg);
    fStride = (fPixelSize * width) / 8;
    fWidth = width;
    fHeight = height;
    
    if (numLevels > 0) {
        fNumLevels = numLevels;
    } else {
        fNumLevels = 1;
        while (width > 1 && height > 1) {
            if (width > 1)
                width /= 2;
            if (height > 1)
                height /= 2;
            numLevels++;
        }
    }

    fCompressionType = compType;
    if (compType == kUncompressed || compType == kJPEGCompression) {
        fUncompressedInfo.fType = format;
    } else {
        fDXInfo.fCompressionType = format;
        fDXInfo.fBlockSize = (format != DirectXInfo::kDXT1 ? 1 : 0);
        if (format == DirectXInfo::kDXT1) {
            fFlags &= ~kAlphaChannelFlag;
            fFlags |= kAlphaBitFlag;
        } else {
            fFlags &= ~kAlphaBitFlag;
            fFlags |= kAlphaChannelFlag;
        }
    }

    if (fLevelSizes != NULL)
        delete[] fLevelSizes;
    fLevelSizes = NULL;
    IBuildLevelSizes();
    fTotalSize = 0;
    for (size_t i=0; i<fNumLevels; i++)
        fTotalSize += fLevelSizes[i];
    fImageData = new unsigned char[fTotalSize];
    memset(fImageData, 0, fTotalSize);
    //setCurrLevel(0);
}

void plMipmap::setConfig(unsigned int cfg) {
    switch (cfg) {
    case kColor8Config:
        fPixelSize = 8;
        fSpace = 3;
        fFlags = kNoFlag;
        break;
    case kGray44Config:
        fPixelSize = 8;
        fSpace = 2;
        fFlags = kAlphaChannelFlag;
        break;
    case kGray4Config:
        fPixelSize = 4;
        fSpace = 2;
        fFlags = kNoFlag;
        break;
    case kGray8Config:
        fPixelSize = 8;
        fSpace = 1;
        fFlags = kNoFlag;
        break;
    case kRGB16Config:
        fPixelSize = 16;
        fSpace = 1;
        fFlags = kAlphaBitFlag;
        break;
    case kRGB32Config:
        fPixelSize = 32;
        fSpace = 1;
        fFlags = kNoFlag;
    case kARGB32Config:
        fPixelSize = 32;
        fSpace = 1;
        fFlags = kAlphaChannelFlag;
        break;
    }
}

const void* plMipmap::getJPEGData() { return fJPEGData; }
unsigned int plMipmap::getJPEGSize() { return fJPEGSize; }
const void* plMipmap::getAlphaData() { return fAlphaData; }
unsigned int plMipmap::getAlphaSize() { return fAlphaSize; }

void plMipmap::readData(hsStream* S) {
    plBitmap::readData(S);
    
    fWidth = S->readInt();
    fHeight = S->readInt();
    fStride = S->readInt();
    fTotalSize = S->readInt();
    fNumLevels = S->readByte();

    if (fImageData != NULL) {
        delete[] fImageData;
        fImageData = NULL;
    }
    if (fTotalSize == 0)
        return;

    IBuildLevelSizes();
    fImageData = new unsigned char[fTotalSize];
    switch (fCompressionType) {
    case kJPEGCompression:
        IReadJPEGImage(S);
        break;
    case kDirectXCompression:
        S->read(fTotalSize, fImageData);
        break;
    case kUncompressed:
        IReadRawImage(S);
        break;
    }
}

void plMipmap::writeData(hsStream* S) {
    plBitmap::writeData(S);

    S->writeInt(fWidth);
    S->writeInt(fHeight);
    S->writeInt(fStride);
    S->writeInt(fTotalSize);
    S->writeByte(fNumLevels);

    if (fTotalSize == 0)
        return;

    switch (fCompressionType) {
    case kJPEGCompression:
        IWriteJPEGImage(S);
        break;
    case kDirectXCompression:
        S->write(fTotalSize, fImageData);
        break;
    case kUncompressed:
        IWriteRawImage(S);
        break;
    }
}

void plMipmap::IPrcWrite(pfPrcHelper* prc) {
    plBitmap::IPrcWrite(prc);

    prc->startTag("Metrics");
    prc->writeParam("Width", fWidth);
    prc->writeParam("Height", fHeight);
    prc->writeParam("Stride", fStride);
    prc->writeParam("TotalSize", fTotalSize);
    prc->writeParam("MipLevels", fNumLevels);
    prc->endTag(true);

    if (fCompressionType == kJPEGCompression) {
        prc->startTag("JPEG");
        prc->writeParam("ImageRLE", fJPEGData == NULL);
        prc->writeParam("AlphaRLE", fAlphaData == NULL);
        prc->endTag();
        
        prc->writeSimpleTag("Image");
        if (!prc->isExcluded(pfPrcHelper::kExcludeTextureData)) {
            if (fJPEGData == NULL)
                prc->writeHexStream(fJPEGDataRLE->fTotalSize, fJPEGDataRLE->fImageData);
            else
                prc->writeHexStream(fJPEGSize, fJPEGData);
        } else {
            prc->writeComment("Texture data excluded");
        }
        prc->closeTag();    // Image
        prc->writeSimpleTag("Alpha");
        if (!prc->isExcluded(pfPrcHelper::kExcludeTextureData)) {
            if (fAlphaData == NULL)
                prc->writeHexStream(fAlphaDataRLE->fTotalSize, fAlphaDataRLE->fImageData);
            else
                prc->writeHexStream(fAlphaSize, fAlphaData);
        } else {
            prc->writeComment("Texture data excluded");
        }
        prc->closeTag();    // Alpha
        
        prc->closeTag();    // JPEG
    } else {
        prc->writeSimpleTag("DDS");
        prc->writeHexStream(fTotalSize, fImageData);
        prc->closeTag();
    }
}

void plMipmap::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "Metrics") {
        fWidth = tag->getParam("Width", "0").toUint();
        fHeight = tag->getParam("Height", "0").toUint();
        fStride = tag->getParam("Stride", "0").toUint();
        fTotalSize = tag->getParam("TotalSize", "0").toUint();
        fNumLevels = tag->getParam("MipLevels", "0").toUint();
    } else if (tag->getName() == "JPEG") {
        if (tag->getParam("ImageRLE", "false").toBool()) {
            fJPEGDataRLE = new plMipmap(fWidth, fHeight, kARGB32Config, 1, 0, 0);
            tag->readHexStream(fJPEGDataRLE->fTotalSize, fJPEGDataRLE->fImageData);
        } else {
            fJPEGSize = tag->getContents().getSize();
            fJPEGData = new unsigned char[fJPEGSize];
            tag->readHexStream(fJPEGSize, fJPEGData);
        }
        if (tag->getParam("AlphaRLE", "false").toBool()) {
            fAlphaDataRLE = new plMipmap(fWidth, fHeight, kARGB32Config, 1, 0, 0);
            tag->readHexStream(fAlphaDataRLE->fTotalSize, fAlphaDataRLE->fImageData);
        } else {
            fAlphaSize = tag->getContents().getSize();
            fAlphaData = new unsigned char[fAlphaSize];
            tag->readHexStream(fAlphaSize, fAlphaData);
        }
    } else if (tag->getName() == "DDS") {
        tag->readHexStream(fTotalSize, fImageData);
    } else {
        plBitmap::IPrcParse(tag, mgr);
    }
}

void plMipmap::IBuildLevelSizes() {
    if (fLevelSizes != NULL)
        delete[] fLevelSizes;
    fLevelSizes = new unsigned int[fNumLevels];
    memset(fLevelSizes, 0, fNumLevels * sizeof(unsigned int));

    unsigned int curWidth = fWidth;
    unsigned int curStride = fStride;
    unsigned int curHeight = fHeight;

    if (fNumLevels <= 0) return;
    for (size_t i=0; i<fNumLevels; i++) {
        if (fCompressionType > 2) return;
        if (fCompressionType == kDirectXCompression &&
            !((curHeight | curWidth) & 3))
            fLevelSizes[i] = (fDXInfo.fBlockSize * curHeight * curWidth) / 16;
        else
            fLevelSizes[i] = curStride * curHeight;
        if (curWidth > 1) {
            curWidth /= 2;
            curStride /= 2;
        }
        if (curHeight > 1)
            curHeight /= 2;
    }
}

plMipmap* plMipmap::IReadRLEImage(hsStream* S) {
    plMipmap* img = new plMipmap(fWidth, fHeight, kARGB32Config, 1, 0, 0);
    int* dataPtr = (int*)img->fImageData;
    while (true) {
        size_t count = S->readInt();
        int data = S->readInt();
        if (count == 0) break;
        for (size_t i=0; i<count; i++) {
            *dataPtr = data;
            dataPtr++;
        }
    }
    return img;
}

void plMipmap::IWriteRLEImage(hsStream* S, plMipmap* img) {
    int* dataPtr = (int*)img->fImageData;
    int data = *dataPtr;
    size_t count = 0;
    for (size_t i=0; i<(fLevelSizes[0] / 4); i++) {
        if ((*dataPtr & 0x00FFFFFF) != data) {
            S->writeInt(count);
            S->writeInt(data);
            count = 0;
            data = *dataPtr;
        }
        count++;
        dataPtr++;
    }
    S->writeInt(count);
    S->writeInt(data);
    
    // Stop indicator
    S->writeInt(0);
    S->writeInt(0);
}

void plMipmap::IReadJPEGImage(hsStream* S) {
    char rleFlag = S->readByte();

    if (rleFlag & kColorDataRLE) {
        fJPEGData = NULL;
        fJPEGDataRLE = IReadRLEImage(S);
    } else {
        fJPEGDataRLE = NULL;
        fJPEGSize = S->readInt();
        fJPEGData = new unsigned char[fJPEGSize];
        S->read(fJPEGSize, fJPEGData);
    }

    if (rleFlag & kAlphaDataRLE) {
        fAlphaData = NULL;
        fAlphaDataRLE = IReadRLEImage(S);
    } else {
        fAlphaDataRLE = NULL;
        fAlphaSize = S->readInt();
        fAlphaData = new unsigned char[fAlphaSize];
        S->read(fAlphaSize, fAlphaData);
    }
}

void plMipmap::IWriteJPEGImage(hsStream* S) {
    char rleFlag = 0;
    if (fJPEGData == NULL)
        rleFlag |= kColorDataRLE;
    if (fAlphaData == NULL)
        rleFlag |= kAlphaDataRLE;
    S->writeByte(rleFlag);

    if (rleFlag & kColorDataRLE) {
        IWriteRLEImage(S, fJPEGDataRLE);
    } else {
        S->writeInt(fJPEGSize);
        S->write(fJPEGSize, fJPEGData);
    }

    if (rleFlag & kAlphaDataRLE) {
        IWriteRLEImage(S, fAlphaDataRLE);
    } else {
        S->writeInt(fAlphaSize);
        S->write(fAlphaSize, fAlphaData);
    }
}

void plMipmap::IReadRawImage(hsStream* S) {
    unsigned char* dataPtr = fImageData;
    if (fPixelSize == 32) {
        if (fNumLevels <= 0) return;
        for (size_t i=0; i<fNumLevels; i++) {
            S->readInts(fLevelSizes[i] / 4, (hsUint32*)dataPtr);
            dataPtr += fLevelSizes[i];
        }
    } else if (fPixelSize == 16) {
        if (fNumLevels <= 0) return;
        for (size_t i=0; i<fNumLevels; i++) {
            S->readShorts(fLevelSizes[i] / 2, (hsUint16*)dataPtr);
            dataPtr += fLevelSizes[i];
        }
    } else {
        throw hsBadParamException(__FILE__, __LINE__);
    }
}

void plMipmap::IWriteRawImage(hsStream* S) {
    unsigned char* dataPtr = fImageData;
    if (fPixelSize == 32) {
        if (fNumLevels <= 0) return;
        for (size_t i=0; i<fNumLevels; i++) {
            S->writeInts(fLevelSizes[i] >> 2, (hsUint32*)dataPtr);
            dataPtr += fLevelSizes[i];
        }
    } else if (fPixelSize == 16) {
        if (fNumLevels <= 0) return;
        for (size_t i=0; i<fNumLevels; i++) {
            S->writeShorts(fLevelSizes[i] >> 1, (hsUint16*)dataPtr);
            dataPtr += fLevelSizes[i];
        }
    } else {
        throw hsBadParamException(__FILE__, __LINE__);
    }
}

void plMipmap::IRecombineAlpha(plMipmap* alphaImg) {
    if (fUncompressedInfo.fType == UncompressedInfo::kRGB8888) {
        for (size_t i=0; i<((fTotalSize-1)/4)+1; i++)
            fImageData[(i*4)+3] = alphaImg->fImageData[(i*4)+2];
    }
    fFlags |= kAlphaChannelFlag;
}

plMipmap* plMipmap::ISplitAlpha() {
    plMipmap* alpha = new plMipmap();
    alpha->CopyFrom(this);
    memset(alpha->fImageData, 0, alpha->fTotalSize);
    if (fUncompressedInfo.fType == UncompressedInfo::kRGB8888) {
        for (size_t i=((fTotalSize-1)/4)+1; i>0; i--)
            alpha->fImageData[(i*4)+2] = fImageData[(i*4)+3];
    }
    return alpha;
}

void plMipmap::CopyFrom(plMipmap* src) {
    if (fImageData != NULL)
        delete[] fImageData;
    
    fWidth = src->fWidth;
    fHeight = src->fHeight;
    fStride = src->fStride;
    fPixelSize = src->fPixelSize;
    fFlags = src->fFlags;
    fSpace = src->fSpace;
    fCompressionType = src->fCompressionType;
    fTotalSize = src->fTotalSize;
    fImageData = new unsigned char[fTotalSize];
    memcpy(fImageData, src->fImageData, fTotalSize);
    fNumLevels = src->fNumLevels;
    if (fCompressionType == kUncompressed ||
        fCompressionType == kJPEGCompression)
        fUncompressedInfo.fType = src->fUncompressedInfo.fType;
    else if (fCompressionType == kDirectXCompression) {
        fDXInfo.fCompressionType = src->fDXInfo.fCompressionType;
        fDXInfo.fBlockSize = src->fDXInfo.fCompressionType;
    }
    IBuildLevelSizes();
    // if (GetDeviceRef()) GetDeviceRef()->fFlags |= kDirty;
}

void plMipmap::ICopyImage(plMipmap* src) {
    if (fImageData != NULL)
        delete[] fImageData;

    fWidth = src->fWidth;
    fHeight = src->fHeight;
    fStride = src->fStride;
    fPixelSize = src->fPixelSize;
    fTotalSize = src->fTotalSize;
    fImageData = new unsigned char[fTotalSize];
    memcpy(fImageData, src->fImageData, fTotalSize);
    IBuildLevelSizes();
}