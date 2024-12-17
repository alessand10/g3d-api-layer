cmake_minimum_required(VERSION 3.30)

add_subdirectory(Source)

add_library(g3d-api-layer INTERFACE
    Include/g3d-api-layer.h
)

target_link_libraries( g3d-api-layer
    PUBLIC INTERFACE G3DAPI
)

target_include_directories(g3d-api-layer
    PUBLIC INTERFACE Include
)
