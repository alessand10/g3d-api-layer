#include "g3d-execution/g3d-execution-factory-vk.h"
#include "g3d-execution/g3d-executor-vk.h"
#include "g3d-execution/g3d-executor-render-target-info-vk.h"
#include "g3d-execution/g3d-command-list-allocator-vk.h"

IG3DExecutor_T *G3DExecutionFactoryVk::createExecutor(IG3DDevice_T* device, uint32_t supportedOperationTypes)
{
    return new G3DExecutorVk(device, supportedOperationTypes);
}

IG3DExecutorRenderTargetInfo_T *G3DExecutionFactoryVk::createFrameInfo()
{
    return new G3DExecutorRenderTargetInfoVk();
}

IG3DCommandListAllocator_T *G3DExecutionFactoryVk::createCommandListAllocator(IG3DDevice_T *device, IG3DExecutor_T *executor)
{
    return new G3DCommandListAllocatorVk(device, executor);
}
