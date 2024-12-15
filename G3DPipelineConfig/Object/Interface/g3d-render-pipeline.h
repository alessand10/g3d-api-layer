#pragma once


class IG3DRenderPipeline_T
{
    public:
    virtual ~IG3DRenderPipeline_T() = default;
    virtual void cmdBindPipeline(class IG3DCommandList_T* commandList) = 0;

};