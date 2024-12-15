
class IG3DCommandListFactory_T {
    public:
        virtual ~IG3DCommandListFactory_T() = default;
        virtual class IG3DCommandListAllocator_T* createCommandListAllocator(class IG3DDevice_T* device, class IG3DExecutor_T* executor) = 0;
};