#pragma once
#include "g3d-command-list-factory.h"

class G3DCommandListFactoryVk : public IG3DCommandListFactory_T {
    public:
    ~G3DCommandListFactoryVk();
    virtual class IG3DCommandListAllocator_T* createCommandListAllocator(class IG3DDevice_T* device, class IG3DExecutor_T* executor) override;

};