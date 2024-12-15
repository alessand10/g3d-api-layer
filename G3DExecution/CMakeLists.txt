
add_library(G3DExecution 
    Factory/Implementation/g3d-command-list-factory-vk.cpp
    Factory/Implementation/g3d-executor-factory-vk.cpp
    Object/Implementation/g3d-command-list-allocator-vk.cpp
    Object/Implementation/g3d-command-list-vk.cpp
    Object/Implementation/g3d-executor-vk.cpp
)

target_include_directories(G3DExecution 
    PUBLIC Factory/Implementation
    PUBLIC Factory/Interface
    PUBLIC Object/Implementation
    PUBLIC Object/Interface
    )

target_link_libraries (G3DExecution
    PUBLIC G3DSystem
)