cmake_minimum_required(VERSION 3.24)
project(cpp-snippets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TESTS "Enable building and running tests" ON)

add_library(ftl INTERFACE)
target_include_directories(ftl INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/data-structures>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/iterators>
)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is not set. Defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (ENABLE_TESTS)
    enable_testing()
    include(cmake/FetchGoogleTest.cmake)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
    message(STATUS "Tests are enabled")
else()
    message(STATUS "Tests are disabled.")
endif()

