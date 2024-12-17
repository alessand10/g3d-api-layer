#include "vk-resource-helpers.h"

uint32_t getSuitableMemoryTypeIndex(VkPhysicalDevice physicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags propertyFlags)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    // A bitmask where (1 << i) is set iff memoryProperties.memoryType[i] is supported given the requirements
    for (uint32_t index = 0 ; index < memoryProperties.memoryTypeCount ; index++) {
        // Is memory at index 'index' supported? (indicated by 1 in bitmask), does the memory at that index support all of the desired property flags? 
        if ((memoryRequirements.memoryTypeBits & (1 << index)) && ((memoryProperties.memoryTypes[index].propertyFlags & propertyFlags) == propertyFlags)) {
            return index;
        }
    }

    throw std::runtime_error("Unable to find a suitable memory type");
}

int getVkFormatFromG3DFormat(G3DFormat format)
{
    VkFormat imageFormat;
    switch(format) {
        case G3DFormat::RGBA8_UNORM:
            imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
            break;
        case G3DFormat::BRGA8_UNORM:
            imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
            break;
        case G3DFormat::RGBA16_FLOAT:
            imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
            break;
        case G3DFormat::RGBA32_FLOAT:
            imageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
            break;
    }

    return imageFormat;
}

int getVkFormatSizeFromG3DFormat(G3DFormat format)
{
    int size;
    switch(format) {
        case G3DFormat::RGBA8_UNORM:
            size = 4;
            break;
        case G3DFormat::BRGA8_UNORM:
            size = 4;
            break;
        case G3DFormat::RGBA16_FLOAT:
            size = 8;
            break;
        case G3DFormat::RGBA32_FLOAT:
            size = 16;
            break;
    }

    return size;
}

VkImageAspectFlags vkFromG3DAspectBitmask(G3DImageAspectBitmap bitmap)
{
    VkImageAspectFlags aspectFlags = 0;
    if (bitmap & G3DImageAspect::COLOR) {
        aspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
    }
    if (bitmap & G3DImageAspect::DEPTH) {
        aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    if (bitmap & G3DImageAspect::STENCIL) {
        aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    return aspectFlags;
}

VkShaderStageFlags vkFromG3DShader(G3DShaderBitmap shader)
{
    VkShaderStageFlags stages;
    if (shader & G3DShader::G3DVertexShader)
        stages |= VK_SHADER_STAGE_VERTEX_BIT;
    if (shader & G3DShader::G3DGeometryShader)
        stages |= VK_SHADER_STAGE_GEOMETRY_BIT;
    if (shader & G3DShader::G3DFragmentShader)
        stages |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if (shader & G3DShader::G3DComputeShader)
        stages |= VK_SHADER_STAGE_COMPUTE_BIT;

    return stages;
}

VkDescriptorType vkFromG3DDescriptorType(G3DPipelineBindingType type)
{
    switch (type) {
        case G3DPipelineBindingType::G3DConstantBuffer:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case G3DPipelineBindingType::G3DImageView:
            return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case G3DPipelineBindingType::G3DSampler:
            return VK_DESCRIPTOR_TYPE_SAMPLER;
        default:
            throw std::runtime_error("Un-implemented descriptor type");
    }
}

VkBufferUsageFlags vkFromG3DBufferUsage(G3DBufferUsage usage)
{
    VkBufferUsageFlags flags = 0;
    if (usage & G3DBufferUsage::VERTEX_BUFFER) {
        flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    if (usage & G3DBufferUsage::INDEX_BUFFER) {
        flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    if (usage & G3DBufferUsage::CONSTANT_BUFFER) {
        flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    if (usage & G3DBufferUsage::STORAGE_BUFFER) {
        flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }
    if (usage & G3DBufferUsage::TRANSFER_SOURCE) {
        flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    }
    if (usage & G3DBufferUsage::TRANSFER_DESTINATION) {
        flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    return flags;
}
