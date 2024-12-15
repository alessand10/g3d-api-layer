#pragma once
#include "g3d-defines.h"

class IG3DExecutor_T {
    protected:
    class IG3DDevice_T* m_device;
    G3DOperationTypeBitmap m_supportedOperationTypes;
    IG3DExecutor_T(class IG3DDevice_T* device, G3DOperationTypeBitmap supportedOperationTypes) {
        m_device = device;
        m_supportedOperationTypes = supportedOperationTypes;
    };
    
    public:
    virtual ~IG3DExecutor_T() = default;
    virtual void executeCommandList(class IG3DCommandList_T* commandList, class IG3DExecutorFrameInfo_T* renderWindowInfo) = 0;
};