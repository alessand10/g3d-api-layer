#include "g3d-command-list-allocator-vk.h"
#include "g3d-device-vk.h"
#include "g3d-command-list-vk.h"
#include "g3d-executor-vk.h"

G3DCommandListAllocatorVk::G3DCommandListAllocatorVk(IG3DDevice_T* device, IG3DExecutor_T* executor) :
    IG3DCommandListAllocator_T(device, executor)
{
    G3DExecutorVk* vkExecutor = dynamic_cast<G3DExecutorVk*>(executor);
    VkCommandPoolCreateInfo poolInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = vkExecutor->getSupportedQueueFamilyIndex()
    };

    if (vkCreateCommandPool(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool");
    }
}

IG3DCommandList_T *G3DCommandListAllocatorVk::allocateCommandList()
{
    return new G3DCommandListVk(m_device, this);
}
