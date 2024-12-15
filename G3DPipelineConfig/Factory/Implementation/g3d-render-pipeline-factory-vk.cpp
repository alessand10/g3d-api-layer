#include "g3d-render-pipeline-factory-vk.h"
#include "g3d-render-pipeline-vk.h"
#include "g3d-resource-set-vk.h"
#include "vk-resource-helpers.h"
#include "vulkan/vulkan.hpp"
#include "g3d-device-vk.h"
#include <map>

IG3DResourceSetSignature_T *G3DRenderPipelineFactoryVk::createResourceSetSignature(G3DPipelineBinding *pipelineBindings, uint32_t numBindings, uint32_t maxInstances)
{
    G3DResourceSetSignatureVk* signature = new G3DResourceSetSignatureVk();

    VkDescriptorPoolCreateInfo poolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .maxSets = maxInstances,
    };

    std::map<VkDescriptorType, uint32_t> typeSizes;
    
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (uint32_t i = 0; i < numBindings; i++) {

        // Increment the number of bindings of this type by maxAllocs
        VkDescriptorType descriptorType = vkFromG3DDescriptorType(pipelineBindings[i].type);
        if (typeSizes.find(descriptorType) == typeSizes.end()) {
            typeSizes[descriptorType] = maxInstances;
        }
        else {
            typeSizes[descriptorType] += maxInstances;
        }

        VkDescriptorSetLayoutBinding binding {
            .binding = i,
            .descriptorType = descriptorType,
            .descriptorCount = 1,
            .stageFlags = vkFromG3DShader(pipelineBindings[i].shaderBitmap),
            .pImmutableSamplers = nullptr
        };

        bindings.push_back(binding);
    }

    VkDescriptorSetLayoutCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .bindingCount = numBindings,
        .pBindings = bindings.data()
    };

    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();

    if (vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &signature->m_descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor set layout");
    }

    std::vector<VkDescriptorPoolSize> poolSizes;
    for (auto& [type, size] : typeSizes) {
        VkDescriptorPoolSize poolSize {
            .type = type,
            .descriptorCount = size
        };

        poolSizes.push_back(poolSize);
    }

    poolCreateInfo.pPoolSizes = poolSizes.data();
    poolCreateInfo.poolSizeCount = poolSizes.size();

    if (vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &signature->m_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool");
    }


    return signature;
}

IG3DResourceSetInstance_T *G3DRenderPipelineFactoryVk::createResourceSetInstance(IG3DResourceSetSignature_T *signature)
{
    G3DResourceSetInstanceVk* instance = new G3DResourceSetInstanceVk();
    instance->m_signature = signature;
    instance->m_device = m_device;
    G3DResourceSetSignatureVk* vkSignature = dynamic_cast<G3DResourceSetSignatureVk*>(signature);

    VkDescriptorSetAllocateInfo allocInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = vkSignature->m_descriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &vkSignature->m_descriptorSetLayout
    };

    VkDevice device = dynamic_cast<G3DDeviceVk*>(m_device)->getDevice();
    if (vkAllocateDescriptorSets(device, &allocInfo, &instance->m_descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor set");
    }

    return instance;
}

IG3DRenderPipeline_T *G3DRenderPipelineFactoryVk::createRenderPipeline(G3DRenderPipelineInfo* createInfo,
    IG3DResourceSetSignature_T** supportedSignatures, uint32_t supportedSignatureCount)
{

    G3DRenderPipelineVk* pipeline = new G3DRenderPipelineVk();
    pipeline->m_device = static_cast<G3DDeviceVk*>(createInfo->device)->getDevice();
    pipeline->createShaderModule((void*)createInfo->vertexStage.bytecode, createInfo->vertexStage.bytecodeSize, &pipeline->m_vertexShader);
    pipeline->createShaderModule((void*)createInfo->fragmentStage.bytecode, createInfo->fragmentStage.bytecodeSize, &pipeline->m_fragmentShader);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    };


    std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {};
    for (uint32_t signatureIndex = 0 ; signatureIndex < supportedSignatureCount ; signatureIndex++) {
        descriptorSetLayouts.push_back(dynamic_cast<G3DResourceSetSignatureVk*>(supportedSignatures[signatureIndex])->getDescriptorSetLayout());
    }

    // Create the pipeline layout
    VkPipelineLayoutCreateInfo plCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = VK_NULL_HANDLE,
        .flags = 0U,
        .setLayoutCount = descriptorSetLayouts.size(),
        .pSetLayouts = descriptorSetLayouts.data(),
        .pushConstantRangeCount = 0U,
        .pPushConstantRanges = nullptr,
    };
    
    if (vkCreatePipelineLayout(pipeline->m_device, &plCreateInfo, nullptr, &pipeline->m_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout");
    }

    pipelineCreateInfo.layout = pipeline->m_pipelineLayout;

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = pipeline->m_vertexShader,
        .pName = "main",
        .pSpecializationInfo = nullptr
    };

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = pipeline->m_fragmentShader,
        .pName = "main",
        .pSpecializationInfo = nullptr
    };

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};

    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;

    
    std::vector<VkVertexInputBindingDescription> vertexInputBindingDescription = {};
    std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions = {};

    for (uint32_t bindingIndex = 0U ; bindingIndex < createInfo->vertexStage.vertexBindingCount ; bindingIndex++) {
        uint32_t numAttributes = createInfo->vertexStage.vertexBindings[bindingIndex].inputCount;
        G3DVertexBindingInputs* bindingAttributes = &createInfo->vertexStage.vertexBindings[bindingIndex];


        // Create the binding description
        VkVertexInputBindingDescription bindingDescription {
            .binding = bindingIndex,
            .stride = 0U,
            .inputRate = VkVertexInputRate(bindingAttributes->inputRate == G3DVertexInputRate::PER_VERTEX ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE)
        };
        vertexInputBindingDescription.push_back(bindingDescription);

        // Create the per attribute descriptions for this binding
        uint32_t accumulatedOffset = 0U;
        for (uint32_t attributeIndex = 0U ; attributeIndex < numAttributes ; attributeIndex++) {
            VkVertexInputAttributeDescription attributeDescription {
                .location = attributeIndex,
                .binding = bindingIndex,
                .format = VkFormat(getVkFormatFromG3DFormat(bindingAttributes->vertexInputs[attributeIndex].format)),
                .offset = accumulatedOffset
            };
            vertexInputAttributeDescriptions.push_back(attributeDescription);

            accumulatedOffset += getVkFormatSizeFromG3DFormat(bindingAttributes->vertexInputs[attributeIndex].format);
        }
    
    }
    
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = vertexInputBindingDescription.size(),
        .pVertexBindingDescriptions = vertexInputBindingDescription.data(),
        .vertexAttributeDescriptionCount = vertexInputAttributeDescriptions.size(),
        .pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data()
    };

    pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;

    // Configure the viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) 1920.f;
    viewport.height = (float) 1080.f;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Configure the scissor (can be used to discard rasterizer pixels)
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent.width = 1920.f;
    scissor.extent.height = 1080.f;

    // Since we are dynamically specifying the viewport and scissor struct, theres no need to specify them in the viewport
    // state. We will set these values up later at draw time.
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    viewportState.pViewports = &viewport;
    viewportState.flags = 0U;
    viewportState.pNext = nullptr;

    pipelineCreateInfo.pViewportState = &viewportState;

    // Set up some rasterizer values
    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.lineWidth = 1.0f;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationState.depthBiasEnable = VK_FALSE;
    rasterizationState.depthBiasConstantFactor = 0.0f; // Optional
    rasterizationState.depthBiasClamp = 0.0f; // Optional
    rasterizationState.depthBiasSlopeFactor = 0.0f; // Optional

    pipelineCreateInfo.pRasterizationState = &rasterizationState;

    // Configure some multisampling
    VkPipelineMultisampleStateCreateInfo multisampleState{};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.sampleShadingEnable = VK_FALSE;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleState.minSampleShading = 1.0f; // Optional
    multisampleState.pSampleMask = nullptr; // Optional
    multisampleState.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampleState.alphaToOneEnable = VK_FALSE; // Optional

    pipelineCreateInfo.pMultisampleState = &multisampleState;

    // Configure the blend attachment state
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    // Configure colour blending
    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &colorBlendAttachment;
    colorBlendState.blendConstants[0] = 0.0f; // Optional
    colorBlendState.blendConstants[1] = 0.0f; // Optional
    colorBlendState.blendConstants[2] = 0.0f; // Optional
    colorBlendState.blendConstants[3] = 0.0f; // Optional

    pipelineCreateInfo.pColorBlendState = &colorBlendState;

    VkPipelineDepthStencilStateCreateInfo dsState {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0U,
        .depthTestEnable = createInfo->config.depthTestEnabled,

    };

    pipelineCreateInfo.pDepthStencilState = &dsState;

    // We need to specify some info for our dynamic rendering pipeline
    VkFormat colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
    // Dynamic pipeline create info
    VkPipelineRenderingCreateInfoKHR dpCreateInfo{ 
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
        .pNext = VK_NULL_HANDLE,
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &colorFormat,
    };

    pipelineCreateInfo.pNext = &dpCreateInfo;

    // Create the pipeline
    if (vkCreateGraphicsPipelines(pipeline->m_device, VK_NULL_HANDLE, 1U, &pipelineCreateInfo, nullptr, &pipeline->m_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create graphics pipeline");
    }

    return pipeline;
}