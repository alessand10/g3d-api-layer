#pragma once
#include "g3d-image-view.h"

class G3DImageViewVk : public IG3DImageView_T {
    friend class G3DResourceFactoryVk;
    protected:
    class VkImageView_T* m_imageView;

    public:
    G3DImageViewVk(class IG3DImage_T *image) : IG3DImageView_T(image) {}
    virtual ~G3DImageViewVk() override;
    class VkImageView_T* getImageView() { return m_imageView; }
    class IG3DImage_T* getImage() { return m_image; }
    virtual void destroy() override;
};