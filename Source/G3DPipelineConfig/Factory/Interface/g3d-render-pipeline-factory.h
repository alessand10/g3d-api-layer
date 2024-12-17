#pragma once
#include "g3d-render-pipeline.h"
#include "minimal-type-defines.h"


class IG3DRenderPipelineFactory_T {
    protected:
    class IG3DDevice_T* m_device;
    public:
    IG3DRenderPipelineFactory_T(class IG3DDevice_T* device) : m_device(device) {}
    virtual ~IG3DRenderPipelineFactory_T() = default;
    virtual class IG3DResourceSetSignature_T* createResourceSetSignature(struct G3DPipelineBinding* pipelineBindings, uint32_t numBindings, uint32_t maxInstances) = 0;
    virtual class IG3DResourceSetInstance_T* createResourceSetInstance(IG3DResourceSetSignature_T* signature) = 0;
    virtual class IG3DRenderPipeline_T* createRenderPipeline(struct G3DRenderPipelineInfo* createInfo,
     class IG3DResourceSetSignature_T** supportedSignatures, uint32_t supportedSignatureCount) = 0;
};