project(OptiXDevice)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "/MD")
set(CMAKE_CXX_FLAGS_DEBUG "/MDd")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CUDA_PROPAGATE_HOST_FLAGS 1)
cuda_add_library(toneMap toneMap.cu)
cuda_compile_and_embed(embedded_ptx_code devicePrograms.cu)

add_library(OptiXDevice
        toneMap
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
        ${embedded_ptx_code}
        )