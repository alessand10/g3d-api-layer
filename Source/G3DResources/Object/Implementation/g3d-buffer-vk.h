#pragma once
#include "g3d-buffer.h"

class G3DBufferVk : public IG3DBuffer_T {
    friend class G3DResourceFactoryVk;
    protected:
    class VkBuffer_T* m_buffer;
    class VkDeviceMemory_T* m_deviceMemory;
    int m_vkBufferUsageFlags;

    public: 
    virtual void loadFromHostMemory(const char* data, uint32_t size, uint32_t dstOffset) override;
    virtual void cmdCopyFromBuffer(class IG3DCommandList_T* commandList, IG3DBuffer_T* buffer, uint32_t size, uint32_t srcOffset, uint32_t dstOffset) override;
    virtual VkBuffer_T* getBuffer() { return m_buffer; }
    virtual void cmdBindAsVertexBuffer(class IG3DCommandList_T* commandList, uint32_t binding) override;
    virtual void destroy() override;
};
