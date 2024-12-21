#pragma once
#include "g3d-pipeline-config/g3d-resource-set.h"

class G3DResourceSetSignatureVk : public IG3DResourceSetSignature_T {
    friend class G3DRenderPipelineFactoryVk;
    protected:
    virtual ~G3DResourceSetSignatureVk() {};
    class VkDescriptorSetLayout_T* m_descriptorSetLayout;
    class VkDescriptorPool_T* m_descriptorPool;
    
    public:
    class VkDescriptorSetLayout_T* getDescriptorSetLayout() { return m_descriptorSetLayout; }
    class VkDescriptorPool_T* getDescriptorPool() { return m_descriptorPool; }
};

class G3DResourceSetInstanceVk : public IG3DResourceSetInstance_T {
    friend class G3DRenderPipelineFactoryVk;
    protected:
    virtual ~G3DResourceSetInstanceVk() {};
    class VkDescriptorSet_T* m_descriptorSet;
    class IG3DResourceSetSignature_T* m_signature;
    public:
    virtual void updateBindings(G3DBindingUpdate** updates, uint32_t numUpdates) override;
    virtual void cmdBindResourceSetInstance(class IG3DCommandList_T* commandList, IG3DPipeline_T* renderPipeline) override;
};