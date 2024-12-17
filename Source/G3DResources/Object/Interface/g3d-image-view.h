#pragma once
#include "g3d-device-resource.h"

class IG3DImageView_T : public IG3DDeviceResource_T {
    protected:
    class IG3DImage_T* m_image;

    public:
    IG3DImageView_T(class IG3DImage_T* image) : m_image(image) {}
    virtual ~IG3DImageView_T() = default;
};