#pragma once
#include "g3d-device-resource.h"
#include "g3d-defines.h"


class IG3DImage_T : public IG3DDeviceResource_T {
    protected:
    G3DImageInfo m_imageInfo;

    G3DImageUsageBitmap m_usageBitmap;

    uint32_t m_deviceMemorySize;

    IG3DImage_T(G3DImageInfo* imageInfo) {
        m_imageInfo = *imageInfo;
    }

    public:
    virtual ~IG3DImage_T() = default;
    virtual uint32_t getHeight() { return m_imageInfo.height; }
    virtual uint32_t getWidth() { return m_imageInfo.width; }

    virtual void loadFromHostMemory(const char* data, uint32_t imageByteSize) = 0;

    /**
     * Ensures that the image is ready for it's intended usage
    */
        
    virtual void cmdEnsureImageReady(class IG3DCommandList_T* commandList, G3DImageUsage usage) = 0;

    virtual void cmdCopyFromImage(
        class IG3DCommandList_T* commandList,
        IG3DImage_T* image,
        ImageCopyRegion2D copyRegionSrc = ImageCopyRegion2D{},
        ImageCopyRegion2D copyRegionDst = ImageCopyRegion2D{}
    ) = 0;
};