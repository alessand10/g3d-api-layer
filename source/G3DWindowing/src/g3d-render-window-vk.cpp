#include "g3d-windowing/g3d-render-window-vk.h"
#include "GLFW/glfw3.h"
#include "g3d-system/g3d-device-vk.h"
#include "g3d-execution/g3d-command-list-vk.h"
#include "g3d-resource/g3d-image-vk.h"
#include "g3d-resource/g3d-image-view-vk.h"
#include <vulkan/vulkan.h>
#include "g3d-execution/g3d-executor-render-target-info-vk.h"
#include "g3d-resource/g3d-resource-factory-vk.h"

int G3DRenderWindowVk::getNextAvailableFrame()
{
    m_currentFrameNumber = (m_currentFrameNumber + 1) % m_numFrames;
    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();

    if (m_frameInFlightFences[m_currentFrameNumber] != VK_NULL_HANDLE){
        vkGetFenceStatus(device, m_frameInFlightFences[m_currentFrameNumber]);

        /**
         * Wait for the frame at the current frame number to be available. This is done so that we cycle through
         * the frames in order
         */
        vkWaitForFences(device, 1, &m_frameInFlightFences[m_currentFrameNumber], VK_TRUE, UINT64_MAX);
    }

    // Set render target info for the current frame number
    dynamic_cast<G3DExecutorRenderTargetInfoVk*>(m_renderTargetInfo)->setImageAvailableSemaphore(m_imageAvailableSemaphores[m_currentFrameNumber]);
    dynamic_cast<G3DExecutorRenderTargetInfoVk*>(m_renderTargetInfo)->setRenderingFinishedSemaphore(m_renderFinishedSemaphores[m_currentFrameNumber]);

    // Get the index of the next available swapchain image
    vkAcquireNextImageKHR(device, m_swapchain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrameNumber], VK_NULL_HANDLE, &m_currentFrameIndex);

    return m_currentFrameNumber;
}


void G3DRenderWindowVk::cmdBeginRendering(IG3DCommandList_T* commandList)
{

    dynamic_cast<G3DImageVk*>(m_swapchainImages[m_currentFrameIndex])->cmdTransitionImageLayout(commandList, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    // Set the render target view that we will be rendering to
    dynamic_cast<G3DCommandListVk*>(commandList)->setRenderTargetView(m_swapchainImageViews[m_currentFrameIndex]);

    // Begin dynamic rendering
    VkRenderingAttachmentInfoKHR colorAttachmentInfo{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR, VK_NULL_HANDLE};
    colorAttachmentInfo.clearValue.color = {0.f, 0.f, 0.f, 0.f};
    colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
    colorAttachmentInfo.imageView = dynamic_cast<G3DImageViewVk*>(m_swapchainImageViews[m_currentFrameIndex])->getImageView();
    colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    
    VkRect2D renderArea = {{0, 0}, {int(m_width), int(m_height)}};

    VkRenderingInfoKHR renderInfo {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
        .pNext = VK_NULL_HANDLE,
    };
    renderInfo.colorAttachmentCount = 1U;
    renderInfo.layerCount = 1U;
    renderInfo.pColorAttachments = &colorAttachmentInfo;
    renderInfo.pDepthAttachment = VK_NULL_HANDLE;
    renderInfo.pStencilAttachment = VK_NULL_HANDLE;
    renderInfo.renderArea = renderArea;
    renderInfo.viewMask = 0U;

    ((PFN_vkCmdBeginRenderingKHR)vkCmdBeginRenderingKHR)(
        dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer(), 
        &renderInfo);

    m_frameInFlightFences[m_currentFrameNumber] = dynamic_cast<G3DCommandListVk*>(commandList)->getInUseFence();
}

void G3DRenderWindowVk::cmdEndRendering(IG3DCommandList_T* commandList)
{
    ((PFN_vkCmdEndRenderingKHR)vkCmdEndRenderingKHR)(dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer());
    dynamic_cast<G3DImageVk*>(m_swapchainImages[m_currentFrameIndex])->cmdTransitionImageLayout(commandList, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
}

void G3DRenderWindowVk::processEvents()
{
    glfwPollEvents();
}

void G3DRenderWindowVk::present()
{
    VkSemaphore waitSemaphores[] = {m_renderFinishedSemaphores[m_currentFrameNumber]};
    VkPresentInfoKHR presentInfo {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = VK_NULL_HANDLE,
        .waitSemaphoreCount = 1U,
        .pWaitSemaphores = waitSemaphores,
        .swapchainCount = 1U,
        .pSwapchains = &m_swapchain,
        .pImageIndices = &m_currentFrameIndex,
    };

    vkQueuePresentKHR(
        dynamic_cast<G3DDeviceVk*>(m_device)->getGraphicsQueue(),
        &presentInfo);
}

bool G3DRenderWindowVk::closeRequested()
{
    return glfwWindowShouldClose(m_window);
}
