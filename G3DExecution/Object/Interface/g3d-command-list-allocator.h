/**
 * Represents the memory allocation of a command list. Two command lists created from different allocators
 * are thread safe.
 */
class IG3DCommandListAllocator_T {
    protected:
    class IG3DDevice_T* m_device;
    class IG3DExecutor_T* m_executor;
    IG3DCommandListAllocator_T(IG3DDevice_T* device, IG3DExecutor_T* executor) { 
        m_device = device; 
        m_executor = executor; 
    };
    
    public:
    class IG3DDevice_T* getDevice() { return m_device; }
    virtual ~IG3DCommandListAllocator_T() = default;
    virtual class IG3DCommandList_T* allocateCommandList() = 0;

};