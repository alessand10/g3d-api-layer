#pragma once
#include "inttypes.h"


enum class G3DFormat {
    RGBA8_UNORM,
    BRGA8_UNORM,
    RGBA16_FLOAT,
    RGBA32_FLOAT
};

// Will the host device ever write to or read from the image?
// Will the GPU ever copy to or from the image?
// Will the image be sampled by the GPU?
// Will the image be written to by the GPU?

enum G3DImageUsage {
    MEMORY_TRANSFER_SOURCE = 0b1 << 0U,
    MEMORY_TRANSFER_DESTINATION = 0b1 << 1U,
    SHADER_SAMPLING = 0b1 << 2U,
    SHADER_WRITING = 0b1 << 3U,
    COLOR_RENDER_TARGET = 0b1 << 4U,
    DEPTH_STENCIL_RENDER_TARGET = 0b1 << 5U
};

typedef unsigned long long G3DImageUsageBitmap;

enum G3DOperationType {
    GRAPHICS = 0b1,
    COMPUTE = 0b1 << 1U,
    TRANSFER = 0b1 << 2U
};

typedef unsigned long G3DOperationTypeBitmap;

enum G3DImageAspect {
    COLOR = 0b1,
    DEPTH = 0b1 << 1U,
    STENCIL = 0b1 << 2U
};

typedef unsigned long G3DImageAspectBitmap;

struct G3DImageInfo {
    class IG3DDevice_T* device;
    uint32_t width, height;
    uint32_t mipLevels;
    uint32_t arrayLayers;
    G3DFormat format;
    bool isStaging;
    G3DImageUsageBitmap usageBitmap;
};

struct G3DImageViewInfo {
    uint32_t baseMipLevel;
    uint32_t mipLevels;
    uint32_t baseArrayLayer;
    uint32_t arrayLayers;
    G3DFormat format;
    G3DImageAspectBitmap aspectBitmap;
};

enum G3DPipelineBindingType {
    G3DConstantBuffer = 0b1,
    G3DImageView = 0b1 << 1U,
    G3DSampler = 0b1 << 2U
};

typedef uint32_t G3DPipelineBindingTypeBitmap;

enum G3DShader {
    G3DVertexShader = 0b1,
    G3DGeometryShader = 0b1 << 1U,
    G3DFragmentShader = 0b1 << 2U,
    G3DComputeShader = 0b1 << 3U
};

typedef uint32_t G3DShaderBitmap;

struct G3DPipelineBinding {
    G3DPipelineBindingType type;
    G3DShaderBitmap shaderBitmap;
};

struct G3DBindingUpdate {
    uint32_t binding;
    class IG3DDeviceResource_T* resource;
};

struct G3DImageViewBindingUpdate : public G3DBindingUpdate {
};

enum G3DBufferUsage {
    VERTEX_BUFFER = 0b1,
    INDEX_BUFFER = 0b1 << 1U,
    CONSTANT_BUFFER = 0b1 << 2U,
    STORAGE_BUFFER = 0b1 << 3U,
    TRANSFER_DESTINATION = 0b1 << 4U,
    TRANSFER_SOURCE = 0b1 << 5U


};

typedef uint32_t G3DBufferUsageBitmap;

struct G3DBufferInfo {
    uint32_t requestedSize;
    bool isStaging;
    G3DBufferUsage usage;
};

struct G3DBufferViewInfo {

};

struct ImageCopyRegion2D {
    int32_t x, y;
    uint32_t width, height;
    uint32_t baseArrayLayer = 0U;
    uint32_t layerCount = 1U;
    uint32_t baseMipLevel = 0U;
    uint32_t levelCount = 1U;
    bool isUnset() {
        return width == 0 && height == 0 && x == 0 && y == 0;
    }
};

/**
 * Used to specify if the data bound is per-vertex or per-instance
 */
enum class G3DVertexInputRate {
    PER_VERTEX,
    PER_INSTANCE
};

struct G3DVertexInput {
    int shaderLocation;
    G3DFormat format;
};

struct G3DVertexBindingInputs {
    G3DVertexInput* vertexInputs;
    int inputCount;
    int bindingIndex;
    G3DVertexInputRate inputRate;
};

struct G3DPipelineStageVertex {
    const char* bytecode;
    int bytecodeSize;
    G3DVertexBindingInputs* vertexBindings;
    int vertexBindingCount;
};

struct G3DPipelineStageFragment {
    const char* bytecode;
    int bytecodeSize;
};

struct G3DPipelineState {
    bool depthTestEnabled;
};

struct G3DRenderPipelineInfo {
    class IG3DDevice_T* device;
    G3DPipelineState config;
    G3DPipelineStageVertex vertexStage;
    G3DPipelineStageFragment fragmentStage;
};