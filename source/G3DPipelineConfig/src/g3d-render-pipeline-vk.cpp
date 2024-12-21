#include "g3d-pipeline-config/g3d-render-pipeline-vk.h"
#include "g3d-execution/g3d-command-list-vk.h"
#include "vulkan/vulkan.hpp"

void G3DRenderPipelineVk::cmdBindPipeline(IG3DCommandList_T *commandList)
{
    VkCommandBuffer commandBuffer = dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer();
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}

G3DRenderPipelineVk::~G3DRenderPipelineVk()
{
}