

add_library(G3DSystem 
    Factory/Implementation/g3d-system-factory-vk.cpp
    Object/Implementation/g3d-application-vk.cpp
    Object/Implementation/g3d-device-vk.cpp
)

target_link_libraries(G3DSystem
    PUBLIC G3DBase
)

target_include_directories(G3DSystem
    PUBLIC Include
    PUBLIC Factory/Implementation
    PUBLIC Factory/Interface
    PUBLIC Object/Implementation
    PUBLIC Object/Interface
)
