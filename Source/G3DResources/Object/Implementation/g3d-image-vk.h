#include "g3d-image.h"

class G3DImageVk : public IG3DImage_T {
    friend class G3DResourceFactoryVk;
    protected:

    class VkImage_T* m_image;
    class VkDeviceMemory_T* m_imageMemory;
    int m_imageLayout;

    ~G3DImageVk();

    G3DImageVk(G3DImageInfo* imageInfo);


    static uint32_t getImageUsageFromG3DUsage(G3DImageUsageBitmap usageBitmap);

    public:

    /**
     * Sets the VkImage object and memory associated with this image
     * 
     * @warning Using this function is dangerous because it can lead to undefined behavior if the image that
     * is set does not match the image information that was used to create this image.
     * 
     * @param image The VkImage object
     */
    void setVkImage(class VkImage_T* image, class VkDeviceMemory_T* memory) { m_image = image; }


    /**
     * Loads image data from a host memory buffer.
     * 
     * @note This function is only valid for staging images.
     * @note The image byte size passed in must match the size of the device memory allocated for the image.
     * 
     * @param data The data to load into the image
     * @param imageByteSize The size of the image data in bytes
     */
    virtual void loadFromHostMemory(const char* data, uint32_t imageByteSize) override;

    virtual void cmdEnsureImageReady(class IG3DCommandList_T* commandList, G3DImageUsage usage) override;

    virtual void cmdTransitionImageLayout(
        class IG3DCommandList_T* commandList,
        int newLayout
    );

    virtual void cmdCopyFromImage(
        class IG3DCommandList_T* commandList,
        IG3DImage_T* image,
        ImageCopyRegion2D copyRegionSrc = ImageCopyRegion2D{},
        ImageCopyRegion2D copyRegionDst = ImageCopyRegion2D{}
    ) override;

    virtual int getImageLayout() { return m_imageLayout; };
    virtual void destroy() override;
};