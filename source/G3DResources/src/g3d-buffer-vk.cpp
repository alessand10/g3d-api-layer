#include "g3d-resource/g3d-buffer-vk.h"
#include "g3d-system/g3d-device-vk.h"
#include "g3d-execution/g3d-command-list-vk.h"
#include <vulkan/vulkan.hpp>

void G3DBufferVk::loadFromHostMemory(const char* data, uint32_t size, uint32_t dstOffset)
{
    if (!isStaging) {
        throw std::runtime_error("Cannot load image from host memory if resource is created with staging flag");
    }

    if (size + dstOffset > m_deviceMemorySize) {
        throw std::runtime_error("Copy size and offset exceeds buffer size");
    }

    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();
    void* mappedMemory = nullptr;
    vkMapMemory(device, m_deviceMemory, 0U, m_deviceMemorySize, 0U, &mappedMemory);
    memcpy(mappedMemory, data, m_deviceMemorySize);
    vkUnmapMemory(device, m_deviceMemory);
}

void G3DBufferVk::cmdCopyFromBuffer(IG3DCommandList_T *commandList, IG3DBuffer_T *buffer, uint32_t size, uint32_t srcOffset, uint32_t dstOffset)
{
    VkBufferCopy copyRegion {
        .srcOffset = srcOffset,
        .dstOffset = dstOffset,
        .size = size
    };
    vkCmdCopyBuffer(
        dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer(),
        dynamic_cast<G3DBufferVk*>(buffer)->getBuffer(),
        m_buffer,
        1,
        &copyRegion
    );
}

void G3DBufferVk::cmdBindAsVertexBuffer(IG3DCommandList_T *commandList, uint32_t binding)
{
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(
        dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer(),
        binding,
        1,
        &m_buffer,
        &offset
    );
}

void G3DBufferVk::destroy()
{
    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();
    vkDestroyBuffer(device, m_buffer, nullptr);
    vkFreeMemory(device, m_deviceMemory, nullptr);
}
