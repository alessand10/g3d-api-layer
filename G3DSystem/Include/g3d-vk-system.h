#pragma once
#include <memory>
#include "g3d-device.h"
#include "g3d-application.h"
#include "g3d-system-factory-vk.h"


typedef std::unique_ptr<IG3DSystemFactory_T> IG3DSystemFactory;
typedef std::unique_ptr<IG3DDevice_T> IG3DDevice;
typedef std::unique_ptr<IG3DApplication_T> IG3DApplication;