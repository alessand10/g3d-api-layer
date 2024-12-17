#pragma once
#include "g3d-command-list-allocator.h"
#include "vulkan/vulkan.hpp"

class G3DCommandListAllocatorVk : public IG3DCommandListAllocator_T {
    friend class G3DExecutionFactoryVk;
    VkCommandPool m_commandPool;

    protected:
    virtual ~G3DCommandListAllocatorVk() = default;
    G3DCommandListAllocatorVk(IG3DDevice_T* device, IG3DExecutor_T* executor);

    public:
    virtual class IG3DCommandList_T* allocateCommandList() override;
    VkCommandPool getCommandPool() { return m_commandPool; }

};