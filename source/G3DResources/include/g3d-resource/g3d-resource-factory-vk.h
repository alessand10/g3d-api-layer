#pragma once
#include "g3d-resource/g3d-resource-factory.h"

class G3DResourceFactoryVk : public IG3DResourceFactory_T {
    public:
    G3DResourceFactoryVk(IG3DDevice_T* device) : IG3DResourceFactory_T(device) {}
    
    virtual IG3DImage_T* createImage(G3DImageInfo* imageInfo) override;
    virtual class IG3DImage_T* createUninitializedImage(G3DImageInfo* imageInfo) override;
    virtual class IG3DImageView_T* createImageView(IG3DImage_T* image, G3DImageViewInfo* viewInfo) override;
    virtual class IG3DSampler_T* createSampler() override;
    virtual class IG3DBuffer_T* createBuffer(G3DBufferInfo* bufferInfo) override;
    virtual class IG3DBufferView_T* createBufferView(IG3DBuffer_T* buffer, G3DBufferViewInfo* viewInfo) override;
};