
#include "g3d-system/g3d-system-factory-vk.h"
#include "g3d-system/g3d-application-vk.h"
#include "g3d-system/g3d-device-vk.h"

G3DSystemFactoryVk::~G3DSystemFactoryVk()
{
}

G3DSystemFactoryVk::G3DSystemFactoryVk()
{
}

IG3DApplication_T *G3DSystemFactoryVk::createApplication()
{
    return new G3DApplicationVk();
}

IG3DDevice_T *G3DSystemFactoryVk::createDevice(IG3DApplication_T *application)
{
    return new G3DDeviceVk(application);
}

