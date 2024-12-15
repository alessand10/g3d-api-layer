

add_library(G3DBase INTERFACE
    g3d-defines.h
    g3d-device-resource.h 
    vk-resource-helpers.h
    minimal-type-defines.h
)

target_include_directories(G3DBase
    INTERFACE .
)