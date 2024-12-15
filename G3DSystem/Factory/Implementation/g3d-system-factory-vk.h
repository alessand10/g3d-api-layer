#pragma once
#include "g3d-system-factory.h"

class G3DSystemFactoryVk : public IG3DSystemFactory_T
{
public:
    G3DSystemFactoryVk();
    virtual ~G3DSystemFactoryVk();

    virtual class IG3DApplication_T* createApplication() override;
    virtual class IG3DDevice_T* createDevice(class IG3DApplication_T* application) override;
};