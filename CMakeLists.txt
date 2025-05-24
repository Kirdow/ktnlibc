cmake_minimum_required(VERSION 3.10)
project(ktnlibc C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(ENABLE_TESTS CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

if (ENABLE_TESTS)
    set(UNITY_VERSION 2.5.2)
    find_package(unity ${UNITY_VERSION} QUIET)
    if (NOT unity_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            unity
            URL https://github.com/ThrowTheSwitch/Unity/archive/refs/tags/v${UNITY_VERSION}.tar.gz
        )

        FetchContent_GetProperties(unity)
        if (NOT unity_POPULATED)
            set(FETCHCONTENT_QUIET NO)
            FetchContent_Populate(unity)
            set(UNITY_SOURCE_DIR ${unity_SOURCE_DIR})
            set(UNITY_INCLUDE_DIR ${unity_SOURCE_DIR}/src)
            set(UNITY_SOURCES ${unity_SOURCE_DIR}/src/unity.c)
        endif()
    endif()

    add_library(unity STATIC ${UNITY_SOURCES})
    target_include_directories(unity PUBLIC ${UNITY_INCLUDE_DIR})
endif()

# Source Files
set(SOURCE_FILES 
    src/array_set.c
    src/hashmap.c
    src/vector_list.c
    src/vector_stack.c
)

# Test Source Files
set(SOURCE_TEST_FILES ${SOURCE_FILES})

if (ENABLE_TESTS)
    # Test Files
    set(TEST_SOURCES
        test/main.c
        test/test_array_set.c
        test/test_hashmap.c
        test/test_vector_list.c
        test/test_vector_stack.c
    )
endif()

add_library(ktnlibc ${SOURCE_FILES})
target_include_directories(ktnlibc PUBLIC include)
target_include_directories(ktnlibc PRIVATE src)

if (ENABLE_TESTS)
    add_executable(test_run ${UNITY_SOURCES} ${TEST_SOURCES})
    target_include_directories(test_run PRIVATE test)
    target_link_libraries(test_run ktnlibc unity)

    add_custom_target(test
        COMMAND test_run
        DEPENDS test_run
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

endif()

add_custom_command(
    TARGET ktnlibc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Creating symlink to compile_commands.json"
)



