#pragma once
#include "g3d-render-window.h"

class G3DRenderWindowVk : public IG3DRenderWindow_T {
    friend class G3DRenderWindowFactoryVk;
    class GLFWwindow* m_window;
    class VkSurfaceKHR_T * m_surface;
    class VkSwapchainKHR_T* m_swapchain;
    class IG3DImage_T* m_swapchainImages[4];
    class IG3DImageView_T* m_swapchainImageViews[4];
    class VkFence_T* m_frameInFlightFences[4];
    class VkSemaphore_T* m_imageAvailableSemaphores[4];
    class VkSemaphore_T* m_renderFinishedSemaphores[4];
    class IG3DCommandListAllocator_T* m_commandListAllocator;
    class IG3DCommandList_T* m_imagePreRenderCommandList[4];
    class IG3DCommandList_T* m_imagePostRenderCommandList[4];

    void* vkCmdBeginRenderingKHR;
    void* vkCmdEndRenderingKHR;

    protected:
    virtual ~G3DRenderWindowVk() = default;
    G3DRenderWindowVk() = default;

    public:
    virtual VkSemaphore_T* getImageAvailableSemaphore(uint32_t index) { return m_imageAvailableSemaphores[index]; }
    virtual VkSemaphore_T* getRenderFinishedSemaphore(uint32_t index) { return m_renderFinishedSemaphores[index]; }
    virtual void setFrameInFlightFence(class VkFence_T* fence, uint32_t index) { m_frameInFlightFences[index] = fence;}
    virtual int getNextAvailableFrame() override;
    virtual void cmdBeginRendering(class IG3DCommandList_T* commandList) override;
    virtual void cmdEndRendering(class IG3DCommandList_T* commandList) override;
    virtual void processEvents() override;
    virtual void present() override;
    virtual bool closeRequested() override;
    virtual class IG3DCommandList_T* getPreRenderTransitionCommandList() { return m_imagePreRenderCommandList[m_currentFrameIndex]; }
    virtual class IG3DCommandList_T* getPostRenderTransitionCommandList() { return m_imagePostRenderCommandList[m_currentFrameIndex]; }
    virtual class IG3DImageView_T* getSwapchainImageView(uint32_t index) { return m_swapchainImageViews[index]; }
};