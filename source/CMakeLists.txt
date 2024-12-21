cmake_minimum_required(VERSION 3.30)
set(CXX_STANDARD C++17)
project(G3D_API_LAYER)

add_compile_options(-g -lglfw -lGLU -lGL -lm)

add_subdirectory(G3DSystem)
add_subdirectory(G3DBase)
add_subdirectory(G3DExecution)
add_subdirectory(G3DResources)
add_subdirectory(G3DPipelineConfig)
add_subdirectory(G3DWindowing)


find_package(libjpeg-turbo REQUIRED)
find_package(glfw3 REQUIRED)
find_package(VulkanLoader REQUIRED)
find_package(VulkanHeaders REQUIRED)
find_package(glm REQUIRED)


add_library(g3d-api-layer INTERFACE)

target_link_libraries(g3d-api-layer
    INTERFACE G3DWindowing
    INTERFACE G3DPipelineConfig
    INTERFACE G3DResources
    INTERFACE G3DExecution
    INTERFACE G3DSystem
)

target_include_directories(g3d-api-layer
    INTERFACE include
)