#include "g3d-command-list-vk.h"
#include "g3d-device-vk.h"
#include "g3d-command-list-allocator-vk.h"
#include <iostream>

G3DCommandListVk::G3DCommandListVk(IG3DDevice_T *device, IG3DCommandListAllocator_T *commandAllocator)
{
    m_device = device;
    m_commandListAllocator = commandAllocator;

    VkFenceCreateInfo fenceInfo {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    vkCreateFence(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), &fenceInfo, nullptr, &m_inUseFence);

    VkCommandBufferAllocateInfo allocInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = dynamic_cast<G3DCommandListAllocatorVk*>(m_commandListAllocator)->getCommandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    if (vkAllocateCommandBuffers(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), &allocInfo, &m_commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffer");
    }
}

void G3DCommandListVk::beginRecording()
{
    VkResult fenceStatus = vkGetFenceStatus(dynamic_cast<G3DDeviceVk*>(m_commandListAllocator->getDevice())->getDevice(), m_inUseFence);

    if (fenceStatus == VK_SUCCESS) {
        std::cerr << "Fence is signalled upon trying to record, be sure to call waitUntilFree() before calling beginRecording()" << std::endl;
    }

    VkCommandBufferBeginInfo beginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0U,
        .pInheritanceInfo = nullptr
    };

    if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer");
    }
}

void G3DCommandListVk::waitUntilFree()
{
    vkWaitForFences(dynamic_cast<G3DDeviceVk*>(m_commandListAllocator->getDevice())->getDevice(), 1, &m_inUseFence, VK_TRUE, UINT64_MAX);
    vkResetFences(dynamic_cast<G3DDeviceVk*>(m_commandListAllocator->getDevice())->getDevice(), 1, &m_inUseFence);
}

void G3DCommandListVk::endRecording()
{
    if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to end recording command buffer");
    }
}

void G3DCommandListVk::reset()
{
    
    if (vkResetCommandBuffer(m_commandBuffer, 0U) != VK_SUCCESS) {
        throw std::runtime_error("Failed to reset command buffer");
    }

    //m_currentRenderTargetView = nullptr;
}
