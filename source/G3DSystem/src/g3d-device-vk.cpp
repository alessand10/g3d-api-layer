#include "g3d-system/g3d-device-vk.h"
#include "g3d-system/g3d-application-vk.h"


void G3DDeviceVk::selectPhysicalDevice()
{
    VkInstance instance = dynamic_cast<G3DApplicationVk*>(m_application)->getInstance();
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (isPhysicalDeviceSuitable(device))
        {
            m_physicalDevice = device;
            return;
        }
    }

    throw std::runtime_error("Failed to find a suitable GPU.");
}

bool G3DDeviceVk::isPhysicalDeviceSuitable(VkPhysicalDevice device)
{
    return true; // Return true if the device is suitable, false otherwise
}

int G3DDeviceVk::getSupportedQueueIndex(G3DOperationTypeBitmap supportedOperations)
{
    uint32_t queueFamilyPropertyCount = 0U;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

    /**
     * For a resource to be submitted in an operation on a particular queue family, the queue
     * family must have ownership. If the resource is only ever used in one queue family, it can
     * keep permanent ownership. Otherwise, if the resource needs to be used in two separate queue
     * families, then it's ownership must be transferred, so we prefer queue families that support
     * as many operations as possible.
     * 
     * Preference order:
     * 1. All families supported
     * 2. Graphics + transfer support (no need to change compute families when dealing with)
     */
    
    bool requestedGraphicsSupport = supportedOperations & G3DOperationType::GRAPHICS;
    bool requestedComputeSupport = supportedOperations & G3DOperationType::COMPUTE;
    bool requestedTransferSupport = supportedOperations & G3DOperationType::TRANSFER;
    VkQueue supportedQueue;
    for (uint32_t index = 0U ; index < queueFamilyProperties.size() ; index++) {
        VkQueueFamilyProperties properties = queueFamilyProperties[index];
        
        // Find the operations supported on this queue
        bool deviceGraphicsSupport = properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
        bool deviceComputeSupport = properties.queueFlags & VK_QUEUE_COMPUTE_BIT;
        bool deviceTransferSupport = properties.queueFlags & VK_QUEUE_TRANSFER_BIT;

        if (!deviceGraphicsSupport && requestedGraphicsSupport)
            break;
        if (!deviceComputeSupport && requestedComputeSupport)
            break;
        if (!deviceTransferSupport && requestedTransferSupport)
            break;
        
        return index;
        
    }

    // No sufficient queue family was found
    return -1;
}

void G3DDeviceVk::destroy()
{
    vkDestroyDevice(m_device, nullptr);
}

void G3DDeviceVk::createDevice()
{
    VkDevice device;
    std::vector<const char*> enabledExtensions = {
        "VK_KHR_swapchain",
        "VK_KHR_dynamic_rendering" // Enable dynamic rendering
    };

    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
        .dynamicRendering = VK_TRUE,
    };

    // Create a logical device
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = &dynamic_rendering_feature;
    deviceCreateInfo.enabledExtensionCount = enabledExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device.");
    }

    m_device = device;
}

void G3DDeviceVk::enumerateQueueFamilies()
{
    uint32_t queueFamilyPropertyCount = 0U;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

    /**
     * For a resource to be submitted in an operation on a particular queue family, the queue
     * family must have ownership. If the resource is only ever used in one queue family, it can
     * keep permanent ownership. Otherwise, if the resource needs to be used in two separate queue
     * families, then it's ownership must be transferred, so we prefer queue families that support
     * as many operations as possible.
     * 
     * Preference order:
     * 1. All families supported
     * 2. Graphics + transfer support (no need to change compute families when dealing with)
     */
    

    for (uint32_t index = 0U ; index < queueFamilyProperties.size() ; index++) {
        VkQueueFamilyProperties properties = queueFamilyProperties[index];
        
        // Find the operations supported on this queue
        bool graphicsSupport = properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
        bool computeSupport = properties.queueFlags & VK_QUEUE_COMPUTE_BIT;
        bool transferSupport = properties.queueFlags & VK_QUEUE_TRANSFER_BIT;

        if (graphicsSupport && computeSupport && transferSupport) {
            m_computeQueueFamilyIndex = index;
            m_graphicsQueueFamilyIndex = index;
            m_transferQueueFamilyIndex = index;
            break;
        }
    }

    vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0U, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, m_computeQueueFamilyIndex, 0U, &m_computeQueue);
    vkGetDeviceQueue(m_device, m_transferQueueFamilyIndex, 0U, &m_transferQueue);
}

G3DDeviceVk::G3DDeviceVk(IG3DApplication_T* application){
    m_application = application;
    selectPhysicalDevice();
    createDevice();
    enumerateQueueFamilies();
}

G3DDeviceVk::~G3DDeviceVk()
{
    
}
