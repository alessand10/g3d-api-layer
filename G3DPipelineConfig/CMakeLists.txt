
add_library(G3DPipelineConfig 
    Factory/Implementation/g3d-render-pipeline-factory-vk.cpp
    Object/Implementation/g3d-render-pipeline-vk.cpp
    Object/Implementation/g3d-resource-set-vk.cpp
)

target_link_libraries(G3DPipelineConfig
    PUBLIC G3DSystem
    PUBLIC G3DExecution
    PUBLIC G3DResources
)

target_include_directories(G3DPipelineConfig 
    PUBLIC Factory/Implementation
    PUBLIC Factory/Interface
    PUBLIC Object/Implementation
    PUBLIC Object/Interface
)