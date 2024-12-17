
typedef unsigned int uint32_t;

class IG3DRenderWindow_T {
    protected:
    class IG3DDevice_T* m_device;
    uint32_t m_numFrames;
    uint32_t m_width;
    uint32_t m_height;

    /**
     * Note that this may not be the same as the swapchain frame index, as we want to decouple the swapchain
     * frame index from the frame order
     */
    int m_currentFrameNumber = -1;
    uint32_t m_currentFrameIndex;
    class IG3DExecutorRenderTargetInfo_T* m_renderTargetInfo;

    public:
    virtual ~IG3DRenderWindow_T() = default;

    /**
     * 
     */
    virtual int getNextAvailableFrame() = 0;
    virtual uint32_t getCurrentFrameNumber() { return m_currentFrameNumber; }
    virtual uint32_t getCurrentFrameIndex() { return m_currentFrameIndex; }
    virtual void cmdBeginRendering(class IG3DCommandList_T* commandList) = 0;
    virtual void cmdEndRendering(class IG3DCommandList_T* commandList) = 0;
    virtual void present() = 0;
    virtual void processEvents() = 0;
    virtual bool closeRequested() = 0;
    virtual class IG3DExecutorRenderTargetInfo_T* getRenderTargetInfo() { return m_renderTargetInfo; };
};