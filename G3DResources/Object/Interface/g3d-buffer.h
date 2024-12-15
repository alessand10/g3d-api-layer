#pragma once
#include "g3d-device-resource.h"
#include "minimal-type-defines.h"

class IG3DBuffer_T : public IG3DDeviceResource_T {
    protected:
    uint32_t m_deviceMemorySize;
    bool isStaging;

    public:
    virtual ~IG3DBuffer_T() = default;
    virtual void loadFromHostMemory(const char* data, uint32_t size, uint32_t dstOffset = 0U) = 0;
    virtual uint32_t getDeviceMemorySize() { return m_deviceMemorySize; }
    virtual void cmdCopyFromBuffer(class IG3DCommandList_T* commandList, IG3DBuffer_T* buffer, uint32_t size, uint32_t srcOffset, uint32_t dstOffset) = 0;
    virtual void cmdBindAsVertexBuffer(class IG3DCommandList_T* commandList, uint32_t binding) = 0;
};