#include "g3d-image-view-vk.h"
#include "g3d-device-vk.h"

G3DImageViewVk::~G3DImageViewVk()
{
}

void G3DImageViewVk::destroy()
{
    vkDestroyImageView(dynamic_cast<G3DDeviceVk*>(m_device)->getDevice(), m_imageView, nullptr);
}
