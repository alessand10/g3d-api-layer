#pragma once

class IG3DRenderWindowFactory_T{
    public:
    virtual ~IG3DRenderWindowFactory_T() = default;
    virtual class IG3DRenderWindow_T* createRenderWindow(
        class IG3DApplication_T* application,
        class IG3DDevice_T* device,
        class IG3DResourceFactory_T* resourceFactory,
        int height, int width) = 0;
};