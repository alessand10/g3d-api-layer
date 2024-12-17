

add_library(G3DWindowing 
    Factory/Implementation/g3d-render-window-factory-vk.cpp
    Object/Implementation/g3d-render-window-vk.cpp
)

target_link_libraries(G3DWindowing
    PUBLIC G3DResources
)

target_include_directories(G3DWindowing 
    PUBLIC Factory/Implementation
    PUBLIC Factory/Interface
    PUBLIC Object/Implementation
    PUBLIC Object/Interface
)