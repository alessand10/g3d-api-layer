#include "g3d-resource-set-vk.h"
#include "g3d-image-view-vk.h"
#include "g3d-device-vk.h"
#include "g3d-image-vk.h"
#include "g3d-sampler-vk.h"
#include "g3d-render-pipeline-vk.h"
#include "g3d-command-list-vk.h"
#include "g3d-buffer-vk.h"
#include "g3d-buffer-view-vk.h"
#include "g3d-defines.h"
#include "vulkan/vulkan.hpp"


void G3DResourceSetInstanceVk::updateBindings(G3DBindingUpdate **updates, uint32_t numUpdates)
{
    std::vector<VkWriteDescriptorSet> descriptorWrites = {};
    std::vector<VkDescriptorImageInfo> imageInfos(numUpdates);
    uint32_t numImageInfosUsed = 0;

    std::vector<VkDescriptorBufferInfo> bufferInfos(numUpdates);
    uint32_t numBufferInfosUsed = 0;

    for (int updateIndex = 0 ; updateIndex < numUpdates ; updateIndex++) {
        G3DImageViewVk* imageView = dynamic_cast<G3DImageViewVk*>(updates[updateIndex]->resource);
        if (imageView != nullptr) {

            imageInfos[numImageInfosUsed] = (VkDescriptorImageInfo {
                .sampler = VK_NULL_HANDLE,
                .imageView = imageView->getImageView(),
                .imageLayout = VkImageLayout(dynamic_cast<G3DImageVk*>(imageView->getImage())->getImageLayout())
            });

            descriptorWrites.push_back(VkWriteDescriptorSet {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = m_descriptorSet,
                .dstBinding = updates[updateIndex]->binding,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                .pImageInfo = &imageInfos[numImageInfosUsed],
                .pBufferInfo = nullptr,
                .pTexelBufferView = nullptr
            });
            numImageInfosUsed++;
            continue;
        }

        G3DSamplerVk* sampler = dynamic_cast<G3DSamplerVk*>(updates[updateIndex]->resource);
        if (sampler != nullptr) {
            imageInfos[numImageInfosUsed] = VkDescriptorImageInfo {
                .sampler = sampler->getSampler(),
                .imageView = VK_NULL_HANDLE,
            };
            

            descriptorWrites.push_back(VkWriteDescriptorSet {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = m_descriptorSet,
                .dstBinding = updates[updateIndex]->binding,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
                .pImageInfo = &imageInfos[numImageInfosUsed],
                .pBufferInfo = nullptr,
                .pTexelBufferView = nullptr
            });

            numImageInfosUsed++;
            continue;
        }

        G3DBufferViewVk* bufferView = dynamic_cast<G3DBufferViewVk*>(updates[updateIndex]->resource);
        if (bufferView != nullptr) {
            G3DBufferVk* buffer = dynamic_cast<G3DBufferVk*>(bufferView->getBuffer());

            bufferInfos[numBufferInfosUsed] = VkDescriptorBufferInfo {
                .buffer = buffer->getBuffer(),
                .offset = 0,
                .range = VK_WHOLE_SIZE
            };

            descriptorWrites.push_back(VkWriteDescriptorSet {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = m_descriptorSet,
                .dstBinding = updates[updateIndex]->binding,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pImageInfo = nullptr,
                .pBufferInfo = &bufferInfos[numBufferInfosUsed],
                .pTexelBufferView = nullptr
            });

            numBufferInfosUsed++;
            continue;
        }
        
        throw std::runtime_error("Unimplemented resource type");
    }

    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();

    vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

void G3DResourceSetInstanceVk::cmdBindResourceSetInstance(IG3DCommandList_T *commandList, IG3DRenderPipeline_T* renderPipeline)
{
    VkCommandBuffer commandBuffer = dynamic_cast<G3DCommandListVk*>(commandList)->getCommandBuffer();
    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
     dynamic_cast<G3DRenderPipelineVk*>(renderPipeline)->getPipelineLayout(), 0, 1, &m_descriptorSet, 0, nullptr);
}
