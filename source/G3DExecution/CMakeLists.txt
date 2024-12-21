
add_library(G3DExecution 
    src/g3d-execution-factory-vk.cpp
    src/g3d-command-list-allocator-vk.cpp
    src/g3d-command-list-vk.cpp
    src/g3d-executor-vk.cpp
)

target_include_directories(G3DExecution 
    PUBLIC include
)

target_link_libraries (G3DExecution
    PUBLIC G3DSystem
)