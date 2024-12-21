#include "g3d-resource/g3d-sampler-vk.h"
#include "g3d-system/g3d-device-vk.h"

void G3DSamplerVk::destroy()
{
    vkDestroySampler(dynamic_cast<G3DDeviceVk*>(m_device)->getDevice(), m_sampler, nullptr);
}