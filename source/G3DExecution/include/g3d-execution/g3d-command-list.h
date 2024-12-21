
class IG3DCommandList_T {
    protected:
    class IG3DDevice_T* m_device;

    public:
    virtual ~IG3DCommandList_T() = default;
    virtual void beginRecording() = 0;
    virtual void endRecording() = 0;
    virtual void waitUntilFree() = 0;
    virtual void reset() = 0;
};