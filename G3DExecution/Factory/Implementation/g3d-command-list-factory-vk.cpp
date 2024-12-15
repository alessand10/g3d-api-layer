#include "g3d-command-list-factory-vk.h"
#include "g3d-command-list-allocator-vk.h"

G3DCommandListFactoryVk::~G3DCommandListFactoryVk()
{
}

IG3DCommandListAllocator_T *G3DCommandListFactoryVk::createCommandListAllocator(IG3DDevice_T* device, IG3DExecutor_T* executor)
{
    return new G3DCommandListAllocatorVk(device, executor);
}
