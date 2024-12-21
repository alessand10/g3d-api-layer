#include "g3d-resource/g3d-buffer-view-vk.h"
#include "vulkan/vulkan.hpp"
#include "g3d-system/g3d-device-vk.h"

void G3DBufferViewVk::destroy()
{
    vkDestroyBufferView(dynamic_cast<G3DDeviceVk*>(m_device)->getDevice(), m_bufferView, nullptr);
}