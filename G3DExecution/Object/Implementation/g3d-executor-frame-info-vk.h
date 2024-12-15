#pragma once
#include "g3d-executor-frame-info.h"

class G3DExecutorFrameInfoVk : public IG3DExecutorFrameInfo_T {
    friend class G3DExecutorFactoryVk;
    class VkSemaphore_T* m_imageAvailableSemaphore;
    class VkSemaphore_T* m_renderFinishedSemaphore;
    public:
    virtual ~G3DExecutorFrameInfoVk() {}

    class VkSemaphore_T* getImageAvailableSemaphore() { return m_imageAvailableSemaphore; }
    class VkSemaphore_T* getRenderFinishedSemaphore() { return m_renderFinishedSemaphore; }
    void setImageAvailableSemaphore(class VkSemaphore_T *semaphore) { m_imageAvailableSemaphore = semaphore; }
    void setRenderingFinishedSemaphore(class VkSemaphore_T *semaphore) { m_imageAvailableSemaphore = semaphore; }
};