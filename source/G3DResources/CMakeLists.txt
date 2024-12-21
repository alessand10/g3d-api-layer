


add_library( G3DResources 
    src/g3d-buffer-view-vk.cpp
    src/g3d-resource-factory-vk.cpp
    src/g3d-buffer-vk.cpp
    src/g3d-image-view-vk.cpp
    src/g3d-image-vk.cpp
    src/g3d-sampler-vk.cpp
)

target_link_libraries(G3DResources
    PUBLIC G3DSystem
    PUBLIC G3DExecution
)

target_include_directories(G3DResources 
    PUBLIC include
)
