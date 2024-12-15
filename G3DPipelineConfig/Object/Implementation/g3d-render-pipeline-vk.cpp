#include "g3d-render-pipeline-vk.h"
#include "g3d-command-list-vk.h"
#include "vulkan/vulkan.hpp"

void G3DRenderPipelineVk::createShaderModule(void* bytecode, int bytecodeLength, VkShaderModule_T **shaderModule)
{
    VkShaderModuleCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = bytecodeLength,
        .pCode = (uint32_t*)bytecode
    };

    if (vkCreateShaderModule(m_device, &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }
}

G3DRenderPipelineVk::~G3DRenderPipelineVk()
{

}

void G3DRenderPipelineVk::cmdBindPipeline(IG3DCommandList_T *commandList)
{
    VkCommandBuffer commandBuffer = dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer();

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}
