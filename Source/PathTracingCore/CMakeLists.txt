project(PTLib)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

include_directories(Source)
include_directories(${OptiX_INCLUDE})

## Find all compile target which matched .cpp
file( GLOB LIB_SOURCES Source/*.cpp )

## Find all header which matched .h
file( GLOB LIB_HEADERS Source/*.h )

## Set Lib include and target
add_library( PTLib
        ${LIB_SOURCES}
        ${LIB_HEADERS} )

## Set Lib macro ExportAPI to true
target_compile_definitions(PTLib PUBLIC PT_API_EXPORT=1)

## Add Lib ThirdParty include
target_include_directories(PTLib PUBLIC ${EIGEN3_INCLUDE_DIR} ${OptiX_INCLUDE})
target_include_directories(PTLib PRIVATE ${STB_INCLUDE_DIRS})

target_link_libraries(PTLib
        OptiXDevice
        ${optix_LIBRARY}
        ${CUDA_LIBRARIES}
        ${CUDA_CUDA_LIBRARY}
        )

