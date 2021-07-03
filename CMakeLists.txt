cmake_minimum_required(VERSION 3.19)
project(WJ_PlayGround VERSION 1.0)

## CMAke Version
set(CMAKE_CXX_STANDARD 14)

## Project Custom Path
set(PlayGroundInclude "./Source/HelloCMake/")
set(PTInclude "./Source/PathTracing/Core/")
set(PTApp "./Source/PathTracing/PTApp/")
set(PTMain "./Source/PathTracing/")

## Find ThirdParty
find_package(Eigen3 CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

## Find all compile target which matched .cpp
file( GLOB LIB_SOURCES ${PTInclude}*.cpp )

## Find all header which matched .h
file( GLOB LIB_HEADERS ${PTInclude}*.h )

## Set Lib include and target
add_library( PTLib ${LIB_SOURCES} ${LIB_HEADERS} )

## Set Lib macro ExportAPI to true
target_compile_definitions(PTLib PUBLIC PT_API_EXPORT=1)

## Add Lib ThirdParty include
target_include_directories(PTLib PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(PTLib PRIVATE ${STB_INCLUDE_DIRS})

## Find all target which inside PTApp folder
file( GLOB PT_SOURCES ${PTApp}*.cpp )

## Add target to Main Project
add_executable(WJ_PlayGround ${PT_SOURCES})

## Set Main Project include path
target_include_directories(WJ_PlayGround PUBLIC ${PTInclude})
target_include_directories(WJ_PlayGround PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(WJ_PlayGround PRIVATE ${STB_INCLUDE_DIRS})


## Set Main Project linked libraries
target_link_directories(WJ_PlayGround PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(WJ_PlayGround PTLib Eigen3::Eigen)
