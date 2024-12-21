

add_library(G3DSystem 
    src/g3d-system-factory-vk.cpp
    src/g3d-application-vk.cpp
    src/g3d-device-vk.cpp
)

target_link_libraries(G3DSystem
    PUBLIC G3DBase
)

target_include_directories(G3DSystem
    PUBLIC include
)
