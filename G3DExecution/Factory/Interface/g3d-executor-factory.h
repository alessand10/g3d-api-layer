#pragma once
#include "g3d-defines.h"

class IG3DExecutorFactory_T {
    protected:
    IG3DExecutorFactory_T() {};
    
    public:
    virtual ~IG3DExecutorFactory_T() = default;
    virtual class IG3DExecutor_T* createExecutor(class IG3DDevice_T* device, uint32_t supportedOperationTypes) = 0;
    virtual class IG3DExecutorFrameInfo_T* createFrameInfo() = 0;
};