function(set_compile_warnings TARGET_NAME)
    if(MSVC)
      target_compile_options(${TARGET_NAME} PRIVATE /W4)
    else()
      target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

find_package(GTest REQUIRED)
find_program(VALGRIND "valgrind")
if (VALGRIND)
    message(STATUS "Valgrind found: ${VALGRIND}")
else()
    message(STATUS "Valgrind not found.")
endif()

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_iterator_test.cpp
)
foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE ftl GTest::Main)
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_compile_warnings(${TEST_NAME})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    if (VALGRIND)
        add_test(NAME ${TEST_NAME}_valgrind COMMAND ${VALGRIND}
            --leak-check=full --error-exitcode=1 --track-origins=yes
            $<TARGET_FILE:${TEST_NAME}>)
    endif()
endforeach()

