#pragma once
#include "vulkan/vulkan.hpp"
#include "g3d-defines.h"

uint32_t getSuitableMemoryTypeIndex(VkPhysicalDevice physicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags propertyFlags);

int getVkFormatFromG3DFormat(G3DFormat format);

int getVkFormatSizeFromG3DFormat(G3DFormat format);

VkImageAspectFlags vkFromG3DAspectBitmask(G3DImageAspectBitmap bitmap);

VkShaderStageFlags vkFromG3DShader(G3DShaderBitmap shader);

VkDescriptorType vkFromG3DDescriptorType(G3DPipelineBindingType type);

VkBufferUsageFlags vkFromG3DBufferUsage(G3DBufferUsage usage);