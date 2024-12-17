#pragma once
#include "g3d-executor-render-target-info.h"

class G3DExecutorRenderTargetInfoVk : public IG3DExecutorRenderTargetInfo_T {
    class VkSemaphore_T* m_imageAvailableSemaphore;
    class VkSemaphore_T* m_renderFinishedSemaphore;
    public:
    virtual ~G3DExecutorRenderTargetInfoVk() {}
    virtual VkSemaphore_T* getImageAvailableSemaphore() { return m_imageAvailableSemaphore; }
    virtual VkSemaphore_T* getRenderFinishedSemaphore() { return m_renderFinishedSemaphore; }
    virtual void setImageAvailableSemaphore(VkSemaphore_T* semaphore) { m_imageAvailableSemaphore = semaphore; }
    virtual void setRenderingFinishedSemaphore(VkSemaphore_T* semaphore) { m_renderFinishedSemaphore = semaphore; }
    
    
};