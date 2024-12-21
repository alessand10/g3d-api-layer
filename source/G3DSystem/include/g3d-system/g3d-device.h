/**
 * Represents the implementation of the 3D graphics api on the device
 */


class IG3DDevice_T {

    public:
    virtual ~IG3DDevice_T() = default;
    virtual void destroy() = 0;
};