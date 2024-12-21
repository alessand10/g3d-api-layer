#include "vulkan/vulkan.hpp"
#include "g3d-resource/g3d-image-vk.h"
#include "g3d-system/g3d-device-vk.h"
#include "vk-resource-helpers.h"
#include "g3d-execution/g3d-command-list-vk.h"

G3DImageVk::G3DImageVk(G3DImageInfo* imageInfo) :
    IG3DImage_T(imageInfo)
{

}

uint32_t G3DImageVk::getImageUsageFromG3DUsage(G3DImageUsageBitmap usageBitmap)
{
     VkImageUsageFlags usageFlags = 0;
    if (usageBitmap & G3DImageUsage::MEMORY_TRANSFER_SOURCE) {
        usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if (usageBitmap & G3DImageUsage::MEMORY_TRANSFER_DESTINATION) {
        usageFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    if (usageBitmap & G3DImageUsage::SHADER_SAMPLING) {
        usageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if (usageBitmap & G3DImageUsage::SHADER_WRITING) {
        usageFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
    }
    if (usageBitmap & G3DImageUsage::COLOR_RENDER_TARGET) {
        usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    if (usageBitmap & G3DImageUsage::DEPTH_STENCIL_RENDER_TARGET) {
        usageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }

    return usageFlags;
}

void G3DImageVk::loadFromHostMemory(const char *data, uint32_t imageByteSize)
{
    if (!m_imageInfo.isStaging) {
        throw std::runtime_error("Cannot load image from host memory if image is not a staging image");
    }

    if (imageByteSize != m_deviceMemorySize) {
        throw std::runtime_error("Image byte size does not match the device allocated size");
    }

    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_imageInfo.device)->getDevice();
    void* mappedMemory = nullptr;
    vkMapMemory(device, m_imageMemory, 0U, m_deviceMemorySize, 0U, &mappedMemory);
    memcpy(mappedMemory, data, imageByteSize);
    vkUnmapMemory(device, m_imageMemory);
}

void G3DImageVk::cmdEnsureImageReady( IG3DCommandList_T* commandList, G3DImageUsage usage)
{
    switch (usage) {
        case G3DImageUsage::MEMORY_TRANSFER_SOURCE:
            cmdTransitionImageLayout(dynamic_cast<G3DCommandListVk*>(commandList), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
            break;
        case G3DImageUsage::MEMORY_TRANSFER_DESTINATION:
            cmdTransitionImageLayout(dynamic_cast<G3DCommandListVk*>(commandList), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            break;
        case G3DImageUsage::SHADER_SAMPLING:
            cmdTransitionImageLayout(dynamic_cast<G3DCommandListVk*>(commandList), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            break;
        case G3DImageUsage::SHADER_WRITING:
            cmdTransitionImageLayout(dynamic_cast<G3DCommandListVk*>(commandList), VK_IMAGE_LAYOUT_GENERAL);
            break;
        case G3DImageUsage::COLOR_RENDER_TARGET:
            cmdTransitionImageLayout(dynamic_cast<G3DCommandListVk*>(commandList), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            break;
        case G3DImageUsage::DEPTH_STENCIL_RENDER_TARGET:
            cmdTransitionImageLayout(dynamic_cast<G3DCommandListVk*>(commandList), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
            break;
        default:
            throw std::runtime_error("Non-implemented image usage");
    }
}

void G3DImageVk::cmdTransitionImageLayout(IG3DCommandList_T *commandList, int newLayout)
{
    // Nothing to change, return immediately
    if (m_imageLayout == newLayout) {
        return;
    }
    
    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = 0,
        .dstAccessMask = 0,
        .oldLayout = VkImageLayout(m_imageLayout),
        .newLayout = VkImageLayout(newLayout),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = m_image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = m_imageInfo.mipLevels,
            .baseArrayLayer = 0,
            .layerCount = m_imageInfo.arrayLayers
        }
    };

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    /**
     * An Image memory barrier ensures that the memory access type of the destination stage awaits the
     * memory access type of the source stage
     */

    if (m_imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        // This is the first transition, so we don't need to wait for anything
        barrier.srcAccessMask = 0;

        // Memory writes need to await this barrier's completion
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

        // Shader reads in the vertex shader (and beyond of course) must await this barrier's completion
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        destinationStage = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_GENERAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (m_imageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    }
    else {
        throw std::runtime_error("Un-implemented layout transition");
    }

    vkCmdPipelineBarrier(
        dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer(),
        sourceStage,
        destinationStage,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier
    );

    m_imageLayout = VkImageLayout(newLayout);
}

void G3DImageVk::cmdCopyFromImage(IG3DCommandList_T* commandList,
    IG3DImage_T *image, ImageCopyRegion2D copyRegionSrc, ImageCopyRegion2D copyRegionDst)
{   
    // Ensure that the destination image is ready for copying
    this->cmdTransitionImageLayout(dynamic_cast<G3DCommandListVk*>(commandList), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    // Ensure that the source image is ready for copying
    dynamic_cast<G3DImageVk*>(image)->cmdTransitionImageLayout(
        dynamic_cast<G3DCommandListVk*>(commandList),
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
    );

    if (copyRegionSrc.isUnset()) {
        copyRegionSrc.width = image->getWidth();
        copyRegionSrc.height = image->getHeight();
    }

    if (copyRegionDst.isUnset()) {
        copyRegionDst.width = getWidth();
        copyRegionDst.height = getHeight();
    }

    // Form the region to copy
    VkImageCopy copyRegion {
        .srcSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = copyRegionSrc.baseMipLevel,
            .baseArrayLayer = copyRegionSrc.baseArrayLayer,
            .layerCount = copyRegionSrc.layerCount
        },
        .srcOffset = {copyRegionSrc.x, copyRegionSrc.y, 0},
        .dstSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = copyRegionDst.baseMipLevel,
            .baseArrayLayer = copyRegionDst.baseArrayLayer,
            .layerCount = copyRegionDst.layerCount
        },
        .dstOffset = {copyRegionDst.x, copyRegionDst.y, 0},
        .extent = {copyRegionSrc.width, copyRegionSrc.height, 1}
    };

    // Run the copy command
    vkCmdCopyImage(
        dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer(),
        dynamic_cast<G3DImageVk*>(image)->m_image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        m_image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &copyRegion
    );
}

void G3DImageVk::destroy()
{
    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_imageInfo.device)->getDevice();
    vkDestroyImage(device, m_image, nullptr);
    vkFreeMemory(device, m_imageMemory, nullptr);
}

G3DImageVk::~G3DImageVk()
{
}
