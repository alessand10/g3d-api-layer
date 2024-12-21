#pragma once

class IG3DPipeline_T
{
    protected: 
    class IG3DDevice_T* m_device;
    
    public:
    virtual ~IG3DPipeline_T() = default;
    virtual void cmdBindPipeline(class IG3DCommandList_T* commandList) = 0;

};