#pragma once
#include "g3d-sampler.h"

class G3DSamplerVk : public IG3DSampler_T {
    friend class G3DResourceFactoryVk;
    protected:
    class VkSampler_T* m_sampler;
    public:
    VkSampler_T* getSampler() { return m_sampler; }
    virtual void destroy() override;
};