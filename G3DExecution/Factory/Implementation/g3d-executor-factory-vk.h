#pragma once
#include "g3d-executor-factory.h"

class G3DExecutorFactoryVk : public IG3DExecutorFactory_T {
    public:
    ~G3DExecutorFactoryVk() override = default;
    virtual class IG3DExecutor_T* createExecutor(class IG3DDevice_T* device, uint32_t supportedOperationTypes) override;
    virtual class IG3DExecutorFrameInfo_T* createFrameInfo() override;
};