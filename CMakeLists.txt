cmake_minimum_required(VERSION 3.21)
project(ktnlibc C
    VERSION 1.0.3
    DESCRIPTION "KTN C Library"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(KTNLIBC_IS_MAIN_PROJECT TRUE)
    set(KTNLIBC_TESTS TRUE)
else()
    set(KTNLIBC_IS_MAIN_PROJECT FALSE)
    set(KTNLIBC_TESTS FALSE)
endif()

if (KTNLIBC_TESTS)
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

# Header Files
set(HEADER_FILES
    include/type.h
    include/array_set.h
    include/hashmap.h
    include/vector_list.h
    include/vector_stack.h
)

add_library(ktnlibc ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(ktnlibc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(ktnlibc PUBLIC c_std_23)

include(GNUInstallDirs)

install(TARGETS ktnlibc
    EXPORT ktnlibcTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ktnlibcTargets
    FILE ktnlibcTargets.cmake
    NAMESPACE ktnlibc::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ktnlibc
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ktnlibcConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ktnlibcConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ktnlibcConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ktnlibc
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ktnlibcConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ktnlibcConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ktnlibc
)

if (KTNLIBC_TESTS)
    # Test Files
    set(TEST_SOURCES
        test/main.c
        test/test_array_set.c
        test/test_hashmap.c
        test/test_vector_list.c
        test/test_vector_stack.c
    )

    add_executable(test_run ${UNITY_SOURCES} ${TEST_SOURCES})
    target_include_directories(test_run PRIVATE test)
    target_link_libraries(test_run ktnlibc unity)

    enable_testing()
    add_test(NAME ktnlibc_test COMMAND test_run)
endif()

if (KTNLIBC_IS_MAIN_PROJECT)
    add_custom_command(
        TARGET ktnlibc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Creating symlink to compile_commands.json"
    )
endif()



