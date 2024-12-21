#include "g3d-device.h"
#include "vulkan/vulkan.hpp"
#include "g3d-defines.h"

class G3DDeviceVk : public IG3DDevice_T {
    protected:
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    uint32_t m_graphicsQueueFamilyIndex, m_computeQueueFamilyIndex, m_transferQueueFamilyIndex;
    VkQueue m_graphicsQueue, m_computeQueue, m_transferQueue;

    class IG3DApplication_T* m_application;

    void selectPhysicalDevice();
    static bool isPhysicalDeviceSuitable(VkPhysicalDevice device);
    void createDevice();
    void enumerateQueueFamilies();


    public:
    G3DDeviceVk(class IG3DApplication_T* application);
    virtual ~G3DDeviceVk() override;

    int getSupportedQueueIndex(G3DOperationTypeBitmap supportedOperations);
    uint32_t getGraphicsQueueFamilyIndex() { return m_graphicsQueueFamilyIndex; }
    uint32_t getComputeQueueFamilyIndex() { return m_computeQueueFamilyIndex; }
    uint32_t getTransferQueueFamilyIndex() { return m_transferQueueFamilyIndex; }
    
    VkQueue getGraphicsQueue() { return m_graphicsQueue; }
    VkDevice getDevice() { return m_device; }
    VkPhysicalDevice getPhysicalDevice() { return m_physicalDevice; }
    virtual void destroy() override;
};