#pragma once
#include "g3d-pipeline.h"
#include "minimal-type-defines.h"

class IG3DComputePipeline_T : public IG3DPipeline_T {
    public:
    virtual ~IG3DComputePipeline_T() = default;
    virtual void cmdDispatch(class IG3DCommandList_T* commandList, uint32_t x, uint32_t y, uint32_t z) = 0;
};