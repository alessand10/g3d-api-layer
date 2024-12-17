#pragma once
#include "g3d-execution-factory.h"

class G3DExecutionFactoryVk : public IG3DExecutionFactory_T {
    public:
    ~G3DExecutionFactoryVk() override = default;
    virtual class IG3DExecutor_T* createExecutor(class IG3DDevice_T* device, uint32_t supportedOperationTypes) override;
    virtual class IG3DExecutorRenderTargetInfo_T* createFrameInfo() override;
    virtual class IG3DCommandListAllocator_T* createCommandListAllocator(class IG3DDevice_T* device, class IG3DExecutor_T* executor) override;
};