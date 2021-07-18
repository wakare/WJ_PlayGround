project(OptiXDevice)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(OptiXDevice
        cmake/configure_build_type.cmake
        cmake/configure_optix.cmake
        cmake/FindOptiX.cmake

        optix7.h
        CUDABuffer.h
        OptiXRenderer.h
        gdt/gdt.h
        gdt/math/LinearSpace.h
        gdt/math/AffineSpace.h

        OptiXRenderer.cpp
        gdt/gdt.cpp
        )