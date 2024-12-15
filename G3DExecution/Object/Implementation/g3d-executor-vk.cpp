#include "g3d-executor-vk.h"
#include "g3d-command-list-vk.h"
#include "g3d-executor-frame-info-vk.h"
#include "g3d-device-vk.h"

G3DExecutorVk::~G3DExecutorVk()
{
}

G3DExecutorVk::G3DExecutorVk(IG3DDevice_T *device, G3DOperationTypeBitmap supportedOperationTypes) : IG3DExecutor_T(device, supportedOperationTypes)
{
    m_device = device;
    m_supportedOperationTypes = supportedOperationTypes;
    
    int supportedQueueIndex = dynamic_cast<G3DDeviceVk*>(device)->getSupportedQueueIndex(supportedOperationTypes);
    if (supportedQueueIndex == -1) {
        throw std::runtime_error("No supported queue found for the requested operation types");
    }
    m_supportedQueueFamilyIndex = supportedQueueIndex;
    
}

void G3DExecutorVk::executeCommandList(IG3DCommandList_T *commandList, IG3DExecutorFrameInfo_T* renderWindowInfo)
{   
    G3DCommandListVk* commandListVk = dynamic_cast<G3DCommandListVk*>(commandList);
    // Fetch the queue if we haven't yet
    if (m_queue == VK_NULL_HANDLE) {
        vkGetDeviceQueue(dynamic_cast<G3DDeviceVk*>(m_device)->getDevice(), m_supportedQueueFamilyIndex, 0U, &m_queue);   
    }

    VkSemaphore waitSemaphore = VK_NULL_HANDLE;
    VkSemaphore signalSemaphore = VK_NULL_HANDLE;

    if (renderWindowInfo) {
        waitSemaphore = dynamic_cast<G3DExecutorFrameInfoVk*>(renderWindowInfo)->getImageAvailableSemaphore();
        signalSemaphore = dynamic_cast<G3DExecutorFrameInfoVk*>(renderWindowInfo)->getRenderFinishedSemaphore();
    }
    
    std::vector<VkCommandBuffer> commandBuffers = {};


    VkCommandBuffer commandBuffer = dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer();
    commandBuffers.push_back(commandBuffer);


    VkPipelineStageFlags waitSemaphoreStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = waitSemaphore != VK_NULL_HANDLE ? 1 : 0,
        .pWaitSemaphores = waitSemaphore != VK_NULL_HANDLE ? &waitSemaphore : nullptr,
        .pWaitDstStageMask = waitSemaphore != VK_NULL_HANDLE ? waitSemaphoreStages : nullptr,
        .commandBufferCount = commandBuffers.size(),
        .pCommandBuffers = commandBuffers.data(),
        .signalSemaphoreCount = signalSemaphore != VK_NULL_HANDLE ? 1 : 0,
        .pSignalSemaphores = signalSemaphore ? &signalSemaphore : nullptr
    };


    VkFence fence = dynamic_cast<G3DCommandListVk*>(commandList)->getInUseFence();

    if (vkQueueSubmit(m_queue, 1, &submitInfo, fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit command buffer to queue");
    }

}
