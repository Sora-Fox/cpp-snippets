cmake_minimum_required(VERSION 3.16)
project(cpp-snippets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TESTS "Enable tests build" ON)

add_library(data_structures INTERFACE)
target_include_directories(data_structures INTERFACE ${CMAKE_SOURCE_DIR}/data-structures)

if (ENABLE_TESTS)
    enable_testing()
    message(STATUS "Tests are enabled.")

    find_package(GTest REQUIRED)
    add_executable(tests tests/matrix_test.cpp)
    target_link_libraries(tests PRIVATE data_structures GTest::GTest GTest::Main)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(tests PRIVATE /W4)
    endif()
    add_test(NAME MatrixTests COMMAND tests)

else()
    message(STATUS "Tests are disabled.")
endif()

