cmake_minimum_required(VERSION 3.19)
project(WJ_PlayGround VERSION 1.0)

## CMAke Version
set(CMAKE_CXX_STANDARD 14)

## Project Custom Path
set(PlayGroundInclude "./Source/HelloCMake/")
set(PTInclude "Source/PathTracingCore/Source/")
set(PTApp "Source/PathTracingApp/Source/")
set(PTMain "Source/PathTracingCore/")

## Find ThirdParty
find_package(Eigen3 CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")
add_subdirectory(Source/PathTracingCore)

## Setup OptixDevice Library
set(OptiXDeviceDir ${PROJECT_SOURCE_DIR}/Source/OptiXDevice)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${OptiXDeviceDir}/cmake/")
include(${OptiXDeviceDir}/cmake/configure_build_type.cmake)
include(${OptiXDeviceDir}/cmake/configure_optix.cmake)
mark_as_advanced(CUDA_SDK_ROOT_DIR)
include_directories(${OptiXDeviceDir})
add_subdirectory(${OptiXDeviceDir})

add_subdirectory(Source/PathTracingApp)

add_subdirectory(Source/VKApp)