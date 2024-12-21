#pragma once
#include "g3d-defines.h"


class IG3DResourceFactory_T {
    protected:
    class IG3DDevice_T* m_device;

    public:
    IG3DResourceFactory_T(class IG3DDevice_T* device) { m_device = device; }
    virtual class IG3DImage_T* createImage(G3DImageInfo* imageInfo) = 0;

    /**
     * Sets the info of an image object without creating the API image itself
     * 
     * @note Expected usage of this function is mostly for swapchain images. You are expected to load
     * the image object for the graphics API that is being used. 
     * 
     * @warning If the image that is later set does not match the image info, undefined behavior may occur.
     */
    virtual class IG3DImage_T* createUninitializedImage(G3DImageInfo* imageInfo) = 0;
    virtual class IG3DImageView_T* createImageView(IG3DImage_T* image, G3DImageViewInfo* viewInfo) = 0;

    virtual class IG3DSampler_T* createSampler() = 0;

    virtual class IG3DBuffer_T* createBuffer(G3DBufferInfo* bufferInfo) = 0;

    virtual class IG3DBufferView_T* createBufferView(IG3DBuffer_T* buffer, G3DBufferViewInfo* viewInfo) = 0;
};