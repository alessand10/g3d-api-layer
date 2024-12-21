#pragma once
#include "g3d-pipeline-config/g3d-pipeline.h"

class IG3DRenderPipeline_T : public IG3DPipeline_T {
    public:
    virtual ~IG3DRenderPipeline_T() = default;
    virtual void cmdBindPipeline(IG3DCommandList_T* commandList) = 0;
};