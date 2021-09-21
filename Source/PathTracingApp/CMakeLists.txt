project(PathTracingApp VERSION 1.0)

## CMake Version
set(CMAKE_CXX_STANDARD 14)
set(PTInclude "../PathTracingCore/Source/")
set(OptiXDeviceInclude "../OptiXDevice")

include_directories(Source)
include_directories(${OptiX_INCLUDE})

## Find all target which inside PTAppSource folder
file( GLOB PT_SOURCES Source/*.cpp )
file( GLOB PT_Window PTAppWindow/*.cpp )

## Add target to Main Project
add_executable(PathTracingApp
        ${PT_SOURCES}
        ${PT_Window})

## Set Main Project include path
target_include_directories(PathTracingApp PUBLIC ${PTInclude})
target_include_directories(PathTracingApp PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(PathTracingApp PRIVATE ${STB_INCLUDE_DIRS})
target_include_directories(PathTracingApp PUBLIC ${OptiXDeviceInclude})

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

## Set Main Project linked libraries
target_link_directories(PathTracingApp PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(PathTracingApp
        glfw
        Eigen3::Eigen
        OptiXDevice
        PTLib
        toneMap
        ${optix_LIBRARY}
        ${CUDA_LIBRARIES}
        ${CUDA_CUDA_LIBRARY}
        ${OPENGL_gl_LIBRARY}
        )