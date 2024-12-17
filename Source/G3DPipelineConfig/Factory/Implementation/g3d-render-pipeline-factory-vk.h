#pragma once
#include "g3d-render-pipeline-factory.h"

class G3DRenderPipelineFactoryVk : public IG3DRenderPipelineFactory_T {
    public:
    G3DRenderPipelineFactoryVk(IG3DDevice_T* device) : IG3DRenderPipelineFactory_T(device) {}
    virtual class IG3DResourceSetSignature_T* createResourceSetSignature(struct G3DPipelineBinding* pipelineBindings, uint32_t numBindings, uint32_t maxInstances) override;
    virtual class IG3DResourceSetInstance_T* createResourceSetInstance(IG3DResourceSetSignature_T* signature) override;
    virtual IG3DRenderPipeline_T* createRenderPipeline(G3DRenderPipelineInfo* createInfo,
        IG3DResourceSetSignature_T** supportedSignatures, uint32_t supportedSignatureCount) override;
};