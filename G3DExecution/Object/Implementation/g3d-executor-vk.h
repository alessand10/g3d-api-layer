#pragma once
#include "g3d-executor.h"
#include "vulkan/vulkan.hpp"


class G3DExecutorVk : public IG3DExecutor_T {
    friend class G3DExecutorFactoryVk;
    uint32_t m_supportedQueueFamilyIndex;
    VkQueue m_queue = nullptr;
    public:
    virtual ~G3DExecutorVk();
    uint32_t getSupportedQueueFamilyIndex() { return m_supportedQueueFamilyIndex; }
    G3DExecutorVk(class IG3DDevice_T* device, G3DOperationTypeBitmap supportedOperationTypes);
    virtual void executeCommandList(class IG3DCommandList_T* commandList, IG3DExecutorFrameInfo_T* renderWindowInfo) override;
};