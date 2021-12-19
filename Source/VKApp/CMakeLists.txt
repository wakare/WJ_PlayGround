cmake_minimum_required(VERSION 3.7)
project(VKApp)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

include_directories(Source)
include_directories(${Vulkan_INCLUDE_DIR})

#add_executable(VKApp Source/VKQueryExtensions.cpp)
add_executable(VKApp Source/VKDrawTriangle.cpp)

target_link_libraries(VKApp
        ${Vulkan_LIBRARY}
        glm::glm
        glfw)