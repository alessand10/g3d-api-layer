
add_library(G3DPipelineConfig
    src/g3d-compute-pipeline-vk.cpp
    src/g3d-pipeline-factory-vk.cpp
    src/g3d-render-pipeline-vk.cpp
    src/g3d-resource-set-vk.cpp
)

target_link_libraries(G3DPipelineConfig
    PUBLIC G3DSystem
    PUBLIC G3DExecution
    PUBLIC G3DResources
)

target_include_directories(G3DPipelineConfig 
    PUBLIC include
)