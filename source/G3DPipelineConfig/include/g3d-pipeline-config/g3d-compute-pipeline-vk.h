#pragma once
#include "g3d-pipeline-config/g3d-compute-pipeline.h"

class G3DComputePipelineVk : public IG3DComputePipeline_T {
    friend class G3DRenderPipelineFactoryVk;
    protected:
    class VkDevice_T* m_device = nullptr;
    class VkShaderModule_T* m_computeShader = nullptr;
    class VkPipeline_T* m_pipeline = nullptr;
    class VkPipelineLayout_T* m_pipelineLayout = nullptr;

    public:
    virtual ~G3DComputePipelineVk() override;
    virtual void cmdBindPipeline(IG3DCommandList_T* commandList) override;
    virtual VkPipeline_T* getPipeline() { return m_pipeline; }
    virtual VkPipelineLayout_T* getPipelineLayout() { return m_pipelineLayout; }
    virtual void cmdDispatch(IG3DCommandList_T* commandList, uint32_t x, uint32_t y, uint32_t z) override;
};