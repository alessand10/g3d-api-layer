#include "g3d-render-window-factory-vk.h"
#include "g3d-resource-factory-vk.h"
#include "g3d-render-window-vk.h"
#include "g3d-image-view-vk.h"
#include "g3d-image-vk.h"
#include "g3d-command-list-vk.h"
#include "g3d-command-list-allocator-vk.h"
#include "g3d-application-vk.h"
#include "g3d-device-vk.h"
#include "vulkan/vulkan.hpp"
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <memory>

IG3DRenderWindow_T *G3DRenderWindowFactoryVk_T::createRenderWindow(IG3DApplication_T* application,
    IG3DDevice_T* device, IG3DResourceFactory_T* resourceFactory, int height, int width)
{
    G3DRenderWindowVk* renderWindow = new G3DRenderWindowVk();
    renderWindow->m_height = height;
    renderWindow->m_width = width;
    renderWindow->m_device = device;


    VkInstance instance = dynamic_cast<G3DApplicationVk*>(application)->getInstance();

    // Load the dynamic rendering commands
    renderWindow->vkCmdBeginRenderingKHR = (void*)vkGetInstanceProcAddr(instance, "vkCmdBeginRenderingKHR");
    renderWindow->vkCmdEndRenderingKHR = (void*)vkGetInstanceProcAddr(instance, "vkCmdEndRenderingKHR");

     // Do not create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    renderWindow->m_window = glfwCreateWindow(renderWindow->m_width , renderWindow->m_height, "Blender Render Graph Visualizer", nullptr, nullptr);
    
    if (glfwCreateWindowSurface(instance, renderWindow->m_window, nullptr, &renderWindow->m_surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }

    // Get the physical device
    VkPhysicalDevice m_physicalDevice = dynamic_cast<G3DDeviceVk*>(device)->getPhysicalDevice();

    VkSurfaceCapabilitiesKHR surfaceCapabilities{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, renderWindow->m_surface, &surfaceCapabilities);

    renderWindow->m_numFrames = surfaceCapabilities.minImageCount + 1;

    VkSwapchainCreateInfoKHR createInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = renderWindow->m_surface,
        .minImageCount = renderWindow->m_numFrames,
        .imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = {static_cast<uint32_t>(renderWindow->m_width), static_cast<uint32_t>(renderWindow->m_height)},
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    if (vkCreateSwapchainKHR(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), &createInfo, nullptr, &renderWindow->m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain");
    }

    std::vector<VkImage> swapchainImagesVk = {};
    /**
     * Retrieve the swapchain images
     */
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), renderWindow->m_swapchain, &imageCount, nullptr);
    swapchainImagesVk.resize(imageCount);
    vkGetSwapchainImagesKHR(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), renderWindow->m_swapchain, &imageCount, swapchainImagesVk.data());

    G3DImageInfo swapchainImageInfo {
        .device = device,
        .width = renderWindow->m_width,
        .height = renderWindow->m_height,
        .mipLevels = 1,
        .arrayLayers = 1,
        .format = G3DFormat::BRGA8_UNORM,
        .isStaging = false,
        .usageBitmap = G3DImageUsage::COLOR_RENDER_TARGET
    };

    VkSemaphoreCreateInfo semaphoreInfo {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };

    VkFenceCreateInfo fenceInfo {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };


    // Create the views for the swapchain images
    for (int i = 0; i < imageCount; i++) {
        vkCreateSemaphore(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), &semaphoreInfo, nullptr, &renderWindow->m_imageAvailableSemaphores[i]);
        vkCreateSemaphore(dynamic_cast<G3DDeviceVk*>(device)->getDevice(), &semaphoreInfo, nullptr, &renderWindow->m_renderFinishedSemaphores[i]);

        renderWindow->m_swapchainImages[i] = resourceFactory->createUninitializedImage(&swapchainImageInfo);
        G3DImageViewInfo imageViewInfo {
            .baseMipLevel = 0,
            .mipLevels = 1,
            .baseArrayLayer = 0,
            .arrayLayers = 1,
            .format = G3DFormat::BRGA8_UNORM,
            .aspectBitmap = G3DImageAspect::COLOR
        };
        
        G3DImageVk* imageVk = dynamic_cast<G3DImageVk*>(renderWindow->m_swapchainImages[i]);
        imageVk->setVkImage(swapchainImagesVk[i], nullptr);

        renderWindow->m_swapchainImageViews[i] = dynamic_cast<G3DResourceFactoryVk*>(resourceFactory)->createImageView(
            renderWindow->m_swapchainImages[i],
            &imageViewInfo
        );


    }

    return renderWindow;
}