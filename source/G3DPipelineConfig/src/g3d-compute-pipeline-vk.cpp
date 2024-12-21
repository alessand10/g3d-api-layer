#include "g3d-pipeline-config/g3d-compute-pipeline-vk.h"
#include "g3d-execution/g3d-command-list-vk.h"
#include <vulkan/vulkan.h>
#include "g3d-pipeline-config/g3d-compute-pipeline-vk.h"

void G3DComputePipelineVk::cmdBindPipeline(IG3DCommandList_T *commandList)
{
    VkCommandBuffer commandBuffer = dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer();
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_pipeline);
}

void G3DComputePipelineVk::cmdDispatch(IG3DCommandList_T *commandList, uint32_t x, uint32_t y, uint32_t z)
{
    VkCommandBuffer commandBuffer = dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer();
    vkCmdDispatch(commandBuffer, x, y, z);
}

G3DComputePipelineVk::~G3DComputePipelineVk()
{

}