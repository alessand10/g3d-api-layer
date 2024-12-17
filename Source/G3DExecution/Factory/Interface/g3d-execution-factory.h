#pragma once
#include "g3d-defines.h"

class IG3DExecutionFactory_T {
    protected:
    IG3DExecutionFactory_T() {};
    
    public:
    virtual ~IG3DExecutionFactory_T() = default;
    virtual class IG3DExecutor_T* createExecutor(class IG3DDevice_T* device, uint32_t supportedOperationTypes) = 0;
    virtual class IG3DExecutorRenderTargetInfo_T* createFrameInfo() = 0;
    virtual class IG3DCommandListAllocator_T* createCommandListAllocator(class IG3DDevice_T* device, class IG3DExecutor_T* executor) = 0;
};