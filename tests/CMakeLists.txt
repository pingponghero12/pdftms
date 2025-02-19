set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

include_directories(include)

find_package(GTest REQUIRED)

add_executable(pdftms_tests
    test_utils.cpp
    test_config.cpp
)

target_include_directories(pdftms_tests

    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(pdftms_tests
    PRIVATE
        yaml-cpp
        GTest::GTest
        GTest::Main
)

target_sources(pdftms_tests

    PRIVATE 
        ${CMAKE_SOURCE_DIR}/src/commands.cpp
        ${CMAKE_SOURCE_DIR}/src/config.cpp
        ${CMAKE_SOURCE_DIR}/src/utils.cpp
        ${CMAKE_SOURCE_DIR}/src/fs.cpp
)

enable_testing()
add_test(
    NAME pdftms_tests
    COMMAND pdftms_tests
)
