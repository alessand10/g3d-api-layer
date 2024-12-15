#pragma once

class IG3DDeviceResource_T {
    protected:
    class IG3DDevice_T* m_device;
    public:
    virtual ~IG3DDeviceResource_T() = default;
    virtual IG3DDevice_T* getDevice() { return m_device; }
    virtual void destroy() = 0;
};