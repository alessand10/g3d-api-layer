#pragma once

class IG3DApplication_T {
    public:
    virtual ~IG3DApplication_T() = default;
    virtual void destroy() = 0;
};