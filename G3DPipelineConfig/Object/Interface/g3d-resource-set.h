#pragma once
#include "minimal-type-defines.h"



class IG3DResourceSetSignature_T {
    public:
    virtual ~IG3DResourceSetSignature_T() = default;
};

class IG3DResourceSetInstance_T {
    protected:
    class IG3DDevice_T* m_device;
    public:
    virtual ~IG3DResourceSetInstance_T() = default;
    virtual void updateBindings(struct G3DBindingUpdate** updates, uint32_t numUpdates) = 0;
    virtual void cmdBindResourceSetInstance(class IG3DCommandList_T* commandList, class IG3DRenderPipeline_T* renderPipeline) = 0;
};