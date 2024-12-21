#include "g3d-system/g3d-application-vk.h"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include "GLFW/glfw3.h"


G3DApplicationVk::~G3DApplicationVk()
{
}


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;

}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)

{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void G3DApplicationVk::destroy()
{
    DestroyDebugUtilsMessengerEXT(m_instance, debugMessenger, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

void G3DApplicationVk::setupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
    if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, NULL, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger");
    }

}

G3DApplicationVk::G3DApplicationVk()
{

    // Wayland is broken for Linux/Vulkan, use X11
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    glfwInitHint(GLFW_ANGLE_PLATFORM_TYPE, GLFW_ANGLE_PLATFORM_TYPE_VULKAN);
    
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    VkApplicationInfo appInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "G3D Application",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "G3D Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3
    };

    const char* enabledLayers[] = {
        #ifndef NDEBUG
        "VK_LAYER_KHRONOS_validation"
        #endif
    };
    
    int enabledLayerCount = sizeof(enabledLayers) / sizeof(const char*);

    std::vector<const char*> enabledExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };


    uint32_t glfwExtensionCount = 0U;
    const char** reqExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        enabledExtensions.push_back(reqExtensions[i]);
    }


    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = enabledLayerCount,
        .ppEnabledLayerNames = enabledLayers,
        .enabledExtensionCount = enabledExtensions.size(),
        .ppEnabledExtensionNames = enabledExtensions.data()
    };

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance");
    };

    #ifndef NDEBUG
    setupDebugMessenger();
    #endif
}
