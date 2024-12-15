#pragma once
#include "g3d-buffer-view.h"

class G3DBufferViewVk : public IG3DBufferView_T {
    friend class G3DResourceFactoryVk;
    protected:
    class VkBufferView_T* m_bufferView;
    public:
    virtual ~G3DBufferViewVk() = default;
    virtual VkBufferView_T* getBufferView() { return m_bufferView; };
    virtual IG3DBuffer_T* getBuffer() { return m_buffer; };
    virtual void destroy() override;
};