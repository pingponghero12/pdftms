cmake_minimum_required(VERSION 3.10)
project(pdftms CXX)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

include_directories(include)

find_package(yaml-cpp REQUIRED)

file(GLOB SOURCE "src/*.cpp")

add_executable(pdftms ${SOURCE})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    yaml-cpp
)
