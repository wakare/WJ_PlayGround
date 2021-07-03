cmake_minimum_required(VERSION 3.19)
project(WJ_PlayGround VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

set(PlayGroundInclude "./Source/HelloCMake/")
set(PTInclude "./Source/PathTracing/Core/")
set(PTApp "./Source/PathTracing/PTApp/")
set(PTMain "./Source/PathTracing/")

find_package(Eigen3 CONFIG REQUIRED)

file( GLOB LIB_SOURCES ${PTInclude}*.cpp )
file( GLOB LIB_HEADERS ${PTInclude}*.h )
add_library( PTLib ${LIB_SOURCES} ${LIB_HEADERS} )
target_compile_definitions(PTLib PUBLIC PT_API_EXPORT=1)
target_include_directories(PTLib PUBLIC ${EIGEN3_INCLUDE_DIR})

file( GLOB PT_SOURCES ${PTApp}*.cpp )
add_executable(WJ_PlayGround ${PT_SOURCES})

target_link_directories(WJ_PlayGround PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(WJ_PlayGround PTLib Eigen3::Eigen)

target_include_directories(WJ_PlayGround PUBLIC ${PTInclude})
target_include_directories(WJ_PlayGround PUBLIC ${EIGEN3_INCLUDE_DIR})