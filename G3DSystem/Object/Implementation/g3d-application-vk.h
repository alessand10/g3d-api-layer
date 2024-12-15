#pragma once
#include "g3d-application.h"

class G3DApplicationVk : public IG3DApplication_T {
    friend class G3DSystemFactoryVk;
    protected:
    class VkInstance_T* m_instance;

    G3DApplicationVk();
    
    void setupDebugMessenger();
    class VkDebugUtilsMessengerEXT_T* debugMessenger;

    public:
    virtual ~G3DApplicationVk();

    class VkInstance_T* getInstance() { return m_instance; }
    virtual void destroy() override;
};