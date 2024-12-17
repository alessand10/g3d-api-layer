#pragma once
#include "g3d-render-pipeline.h"

class G3DRenderPipelineVk : public IG3DRenderPipeline_T {
    friend class G3DRenderPipelineFactoryVk;
    protected:
    class VkDevice_T* m_device;
    class VkShaderModule_T* m_vertexShader;
    class VkShaderModule_T* m_fragmentShader;
    class VkPipeline_T* m_pipeline;
    class VkPipelineLayout_T* m_pipelineLayout;

    void createShaderModule(void* bytecode, int bytecodeLength, VkShaderModule_T** shaderModule);

    public:
    virtual ~G3DRenderPipelineVk() override;
    virtual void cmdBindPipeline(IG3DCommandList_T* commandList) override;
    virtual VkPipeline_T* getPipeline() { return m_pipeline; }
    virtual VkPipelineLayout_T* getPipelineLayout() { return m_pipelineLayout; }
};