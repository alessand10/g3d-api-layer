

add_library(G3DWindowing 
    src/g3d-render-window-factory-vk.cpp
    src/g3d-render-window-vk.cpp
)

target_link_libraries(G3DWindowing
    PUBLIC G3DResources
)

target_include_directories(G3DWindowing 
    PUBLIC include
)