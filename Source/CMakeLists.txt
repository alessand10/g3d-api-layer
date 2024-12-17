set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CXX_STANDARD C++17)
cmake_minimum_required(VERSION 3.30)
project(G3DAPI)

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


add_library(G3DAPI INTERFACE)

target_link_libraries( G3DAPI
    PUBLIC INTERFACE G3DWindowing
    PUBLIC INTERFACE G3DPipelineConfig
    PUBLIC INTERFACE G3DResources
    PUBLIC INTERFACE G3DExecution
    PUBLIC INTERFACE G3DSystem
)

