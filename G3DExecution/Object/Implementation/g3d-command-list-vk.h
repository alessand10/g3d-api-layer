#include "g3d-command-list.h"
class G3DCommandListVk : public IG3DCommandList_T {
    friend class G3DCommandListAllocatorVk;
    protected:
    class IG3DCommandListAllocator_T* m_commandListAllocator;
    class VkCommandBuffer_T* m_commandBuffer;
    class IG3DImageView_T* m_currentRenderTargetView;

    /**
     * Is reset when the command list has started recording
     */
    class VkFence_T* m_inUseFence;
    G3DCommandListVk(class IG3DDevice_T* device, class IG3DCommandListAllocator_T* commandAllocator);

    public:

    VkCommandBuffer_T* getCommandBuffer() {
        return m_commandBuffer;
    }

    virtual void beginRecording() override;
    virtual void waitUntilFree() override;
    virtual void endRecording() override;
    virtual void reset() override;
    virtual void setRenderTargetView(IG3DImageView_T* renderTargetView) {
        m_currentRenderTargetView = renderTargetView;
    };
    virtual IG3DImageView_T* getRenderTargetView() { return m_currentRenderTargetView; }
    
    VkFence_T* getInUseFence() {
        return m_inUseFence;
    }


};