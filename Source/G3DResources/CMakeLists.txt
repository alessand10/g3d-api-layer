


add_library( G3DResources 
    Object/Implementation/g3d-buffer-view-vk.cpp
    Factory/Implementation/g3d-resource-factory-vk.cpp
    Object/Implementation/g3d-buffer-vk.cpp
    Object/Implementation/g3d-image-view-vk.cpp
    Object/Implementation/g3d-image-vk.cpp
    Object/Implementation/g3d-sampler-vk.cpp
)

target_link_libraries(G3DResources
    PUBLIC G3DSystem
    PUBLIC G3DExecution
)

target_include_directories(G3DResources 
    PUBLIC Factory/Implementation
    PUBLIC Factory/Interface
    PUBLIC Object/Implementation
    PUBLIC Object/Interface
)
