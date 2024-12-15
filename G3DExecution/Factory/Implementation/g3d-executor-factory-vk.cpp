#include "g3d-executor-factory-vk.h"
#include "g3d-executor-vk.h"
#include "g3d-executor-frame-info-vk.h"

IG3DExecutor_T *G3DExecutorFactoryVk::createExecutor(IG3DDevice_T* device, uint32_t supportedOperationTypes)
{
    return new G3DExecutorVk(device, supportedOperationTypes);
}

IG3DExecutorFrameInfo_T *G3DExecutorFactoryVk::createFrameInfo()
{
    return new G3DExecutorFrameInfoVk();
}
