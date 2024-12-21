#pragma once
#include "g3d-windowing/g3d-render-window-factory.h"

class G3DRenderWindowFactoryVk : public IG3DRenderWindowFactory_T {
    public:
    virtual ~G3DRenderWindowFactoryVk() = default;
    virtual class IG3DRenderWindow_T* createRenderWindow(
        class IG3DApplication_T* application, 
        class IG3DDevice_T* device, 
        class IG3DResourceFactory_T* resourceFactory,
        int height, int width) override;
};