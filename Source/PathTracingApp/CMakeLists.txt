project(PathTracingApp VERSION 1.0)

## CMAke Version
set(CMAKE_CXX_STANDARD 14)
set(PTInclude "../PathTracingCore/Source/")

include_directories(Source)
include_directories(${OptiX_INCLUDE})

## Find all target which inside PTAppSource folder
file( GLOB PT_SOURCES Source/*.cpp )

## Add target to Main Project
add_executable(PathTracingApp ${PT_SOURCES})

## Set Main Project include path
target_include_directories(PathTracingApp PUBLIC ${PTInclude})
target_include_directories(PathTracingApp PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(PathTracingApp PRIVATE ${STB_INCLUDE_DIRS})

## Set Main Project linked libraries
target_link_directories(PathTracingApp PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(PathTracingApp
        PTLib
        Eigen3::Eigen
        OptiXDevice
        ${optix_LIBRARY}
        ${CUDA_LIBRARIES}
        ${CUDA_CUDA_LIBRARY})