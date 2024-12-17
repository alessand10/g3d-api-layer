#include "g3d-resource-factory-vk.h"
#include "g3d-image-vk.h"
#include "g3d-image-view-vk.h"
#include "g3d-device-vk.h"
#include "vulkan/vulkan.hpp"
#include "vk-resource-helpers.h"
#include "g3d-sampler-vk.h"
#include "g3d-buffer-view-vk.h"
#include "g3d-buffer-vk.h"
#include <map>


IG3DImage_T *G3DResourceFactoryVk::createImage(G3DImageInfo* imageInfo)
{
    G3DImageVk* image = new G3DImageVk(imageInfo);

    image->m_imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkDevice device = dynamic_cast<G3DDeviceVk*>(imageInfo->device)->getDevice();
    
    VkImageCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VkFormat(getVkFormatFromG3DFormat(imageInfo->format)),
        .extent = {imageInfo->width, imageInfo->height, 1U},
        .mipLevels = imageInfo->mipLevels,
        .arrayLayers = imageInfo->arrayLayers,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = imageInfo->isStaging ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL,
        .usage = G3DImageVk::getImageUsageFromG3DUsage(imageInfo->usageBitmap),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0U,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    if (vkCreateImage(device, &createInfo, nullptr, &image->m_image) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image");
    }

    /**
     * ---------    Allocate Memory for the image ----------------
     */

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image->m_image, &memRequirements);

    image->m_deviceMemorySize = memRequirements.size;

    VkMemoryPropertyFlags memoryProperties;
    if (imageInfo->isStaging) {
        memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    else {
        memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }

    uint32_t memTypeIndex = getSuitableMemoryTypeIndex(
        dynamic_cast<G3DDeviceVk*>(image->m_imageInfo.device)->getPhysicalDevice(), memRequirements, memoryProperties);

    VkMemoryAllocateInfo allocInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = memTypeIndex
    };

    if (vkAllocateMemory(device, &allocInfo, nullptr, &image->m_imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate image memory");
    }

    if (vkBindImageMemory(dynamic_cast<G3DDeviceVk*>(imageInfo->device)->getDevice(), image->m_image, image->m_imageMemory, 0U) != VK_SUCCESS) {
        throw std::runtime_error("Failed to bind image memory");
    }

    return image;
}

IG3DImage_T *G3DResourceFactoryVk::createUninitializedImage(G3DImageInfo *imageInfo)
{
    G3DImageVk* image = new G3DImageVk(imageInfo);
    image->m_imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    return image;
}


IG3DImageView_T *G3DResourceFactoryVk::createImageView(IG3DImage_T *image, G3DImageViewInfo *viewInfo)
{
    G3DImageVk* vkImage = dynamic_cast<G3DImageVk*>(image);
    G3DImageViewVk* imageView = new G3DImageViewVk(image);

    VkImageViewCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .image = vkImage->m_image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VkFormat(getVkFormatFromG3DFormat(viewInfo->format)),
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange = {
            .aspectMask = vkFromG3DAspectBitmask(viewInfo->aspectBitmap),
            .baseMipLevel = viewInfo->baseMipLevel,
            .levelCount = viewInfo->mipLevels,
            .baseArrayLayer = viewInfo->baseArrayLayer,
            .layerCount = viewInfo->arrayLayers
        }
    };

    VkDevice device = dynamic_cast<G3DDeviceVk*>(vkImage->m_imageInfo.device)->getDevice();
    if (vkCreateImageView(device, &createInfo, nullptr, &imageView->m_imageView) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image view");
    }

    return imageView;
}


IG3DSampler_T *G3DResourceFactoryVk::createSampler()
{
    G3DSamplerVk* sampler = new G3DSamplerVk();

    VkSamplerCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias = 0.0f,
        .anisotropyEnable = VK_FALSE,
        .maxAnisotropy = 1.0f,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0f,
        .maxLod = 0.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE
    };

    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();
    if (vkCreateSampler(device, &createInfo, nullptr, &sampler->m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create sampler");
    }

    return sampler;   
}

IG3DBuffer_T *G3DResourceFactoryVk::createBuffer(G3DBufferInfo *bufferInfo)
{
    G3DBufferVk* buffer = new G3DBufferVk();
    buffer->m_device = m_device;
    buffer->isStaging = bufferInfo->isStaging;
    buffer->m_vkBufferUsageFlags = vkFromG3DBufferUsage(bufferInfo->usage);

    VkBufferCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = bufferInfo->requestedSize,
        .usage = VkBufferUsageFlags(buffer->m_vkBufferUsageFlags),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr
    };

    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();
    if (vkCreateBuffer(device, &createInfo, nullptr, &buffer->m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer");
    }


    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer->m_buffer, &memRequirements);

    buffer->m_deviceMemorySize = memRequirements.size;

    VkMemoryPropertyFlags memoryProperties;
    if (bufferInfo->isStaging) {
        memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    else {
        memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }

    uint32_t memTypeIndex = getSuitableMemoryTypeIndex(
        dynamic_cast<G3DDeviceVk*>(buffer->m_device)->getPhysicalDevice(), memRequirements, memoryProperties);

    VkMemoryAllocateInfo allocInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = memTypeIndex
    };

    if (vkAllocateMemory(device, &allocInfo, nullptr, &buffer->m_deviceMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory");
    }

    if (vkBindBufferMemory(device, buffer->m_buffer, buffer->m_deviceMemory, 0U) != VK_SUCCESS) {
        throw std::runtime_error("Failed to bind buffer memory");
    }

    return buffer;
}

IG3DBufferView_T *G3DResourceFactoryVk::createBufferView(IG3DBuffer_T *buffer, G3DBufferViewInfo *viewInfo)
{
    G3DBufferViewVk* view = new G3DBufferViewVk();
    view->m_buffer = buffer;
    
    return view;
}
