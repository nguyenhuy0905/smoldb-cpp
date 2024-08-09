# ##############################################################################
# smoldb top level CMakeLists
# ##############################################################################

cmake_minimum_required(VERSION 3.28)
project(
    smoldb
    VERSION 0.1.0
    LANGUAGES C CXX
    HOMEPAGE_URL "https://github.com/nguyenhuy0905/smoldb-cpp"
    DESCRIPTION
        "A simple DB in C++. A \"refactor\" of one of my collab projects")

if(NOT CMAKE_CXX_STANDARD)
    message(STATUS "C++ standard not set, default to C++23")
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT CMAKE_C_STANDARD AND NOT MSVC)
    message(STATUS "C standard not set, default to C23")
    set(CMAKE_C_STANDARD 23)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

# turn off compiler extensions
message(STATUS "Turn off ${CMAKE_CXX_COMPILER_ID} extensions")
message(STATUS "Turn off ${CMAKE_C_COMPILER_ID} extensions")
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(smoldb_compile_opts INTERFACE)
target_compile_features(smoldb_compile_opts
                        INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

message(STATUS "Configure project options")
include(CMakeOptions.cmake)

if(NOT MSVC)
    # there's no such thing as a "C standard" in MSVC
    target_compile_features(smoldb_compile_opts
                            INTERFACE c_std_${CMAKE_C_STANDARD})
endif()

if(smoldb_PACK)
    include(CPack)
    include(cmake/PackConfig.cmake)
endif()

add_subdirectory(src)
add_subdirectory(submodule)

if(smoldb_ENABLE_UNIT_TEST)
    message(STATUS "Unit test enabled")
    add_subdirectory(unittest)
endif()

if(smoldb_ENABLE_FUZZ_TEST)
    message(STATUS "Fuzz test enabled")
    message(STATUS "Checking if libFuzzer linkable")
    include(cmake/CheckFuzzSourceCompile.cmake)
    smoldb_check_fuzz_compile(smoldb_FUZZ_COMPILE)
    if(smoldb_FUZZ_COMPILE)
        add_subdirectory(fuzztest)
    endif()
endif()
