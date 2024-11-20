cmake_minimum_required(VERSION 3.16)
project(cpp_snippets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")
option(ENABLE_TESTS "Enable building and running tests" ON)

add_library(cpp_snippets_lib INTERFACE)
target_include_directories(cpp_snippets_lib INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/data-structures>
)

function(set_common_compile_options target)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE /W4)
    endif()
endfunction()

if (ENABLE_TESTS)
    enable_testing()
    message(STATUS "Tests are enabled.")
    find_package(GTest REQUIRED)
    find_program(VALGRIND "valgrind")

    if (VALGRIND)
        message(STATUS "Valgrind found: ${VALGRIND}")
    else()
        message(WARNING "Valgrind not found.")
    endif()

    file(GLOB TEST_SOURCES "${TESTS_DIR}/*.cpp")
    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests PRIVATE cpp_snippets_lib
        GTest::GTest GTest::Main)
    set_common_compile_options(tests)

    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_FILE})
        target_link_libraries(${TEST_NAME} PRIVATE cpp_snippets_lib 
            GTest::GTest GTest::Main)
        set_common_compile_options(${TEST_NAME})
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        if (VALGRIND)
            add_test(NAME ${TEST_NAME}_valgrind
                COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1 
                $<TARGET_FILE:${TEST_NAME}>)
            set_tests_properties(${TEST_NAME}_valgrind PROPERTIES LABELS "valgrind")
        endif()
    endforeach()
else()
    message(STATUS "Tests are disabled.")
endif()

