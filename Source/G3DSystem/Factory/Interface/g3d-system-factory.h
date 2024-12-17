#pragma once

class IG3DSystemFactory_T 
{
public:
    virtual ~IG3DSystemFactory_T() = default;

    virtual class IG3DDevice_T* createDevice(class IG3DApplication_T* application) = 0;
    virtual class IG3DApplication_T* createApplication() = 0;
};