#pragma once
#include "g3d-pipeline-config/g3d-pipeline-factory.h"

class G3DRenderPipelineFactoryVk : public IG3DPipelineFactory_T {
    public:
    virtual void createShaderModule(void* bytecode, int bytecodeLength, class VkShaderModule_T** shaderModule);
    G3DRenderPipelineFactoryVk(IG3DDevice_T* device) : IG3DPipelineFactory_T(device) {}
    virtual class IG3DResourceSetSignature_T* createResourceSetSignature(struct G3DPipelineBinding* pipelineBindings, uint32_t numBindings, uint32_t maxInstances) override;
    virtual class IG3DResourceSetInstance_T* createResourceSetInstance(IG3DResourceSetSignature_T* signature) override;
    virtual IG3DRenderPipeline_T* createRenderPipeline(G3DRenderPipelineInfo* createInfo,
        IG3DResourceSetSignature_T** supportedSignatures, uint32_t supportedSignatureCount) override;
    virtual IG3DComputePipeline_T* createComputePipeline(struct G3DComputePipelineInfo* createInfo,
    class IG3DResourceSetSignature_T** supportedSignatures, uint32_t supportedSignatureCount) override;
};