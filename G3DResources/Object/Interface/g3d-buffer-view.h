#pragma once
#include "g3d-device-resource.h"

class IG3DBufferView_T : public IG3DDeviceResource_T {
    protected:
    class IG3DBuffer_T* m_buffer;
    public:
    virtual ~IG3DBufferView_T() = default;
    virtual void destroy() = 0;
};