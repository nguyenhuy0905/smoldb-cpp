#
# List of options
#
include(CMakeDependentOption)

# if build type is not set
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("No build type specified! Default to Debug")
    set(CMAKE_BUILD_TYPE
        "Debug"
        CACHE STRING
              "Choose a build type (Debug;Release;MinSizeRel;RelWithDebInfo)")
    set_property(CACHE CMAKE_BUILD_TYPE
                 PROPERTY STRING "Debug;Release;MinSizeRel;RelWithDebInfo")
endif()

#
# Global options
#

# If there's another top-level project, high chance that project already sets
# these options.
option(smoldb_cpp_ENABLE_CCACHE "Use ccache" OFF)
option(smoldb_cpp_ENABLE_LLD "Use lld instead of the compiler-default linker" OFF)
option(smoldb_cpp_ENABLE_LTO "Enable link-time optimization" OFF)
cmake_dependent_option(
    smoldb_cpp_ENABLE_MODULE "Enable modules. Require C++20 and above" OFF
    "${CMAKE_CXX_STANDARD} GREATER_EQUAL 20" OFF)
option(smoldb_cpp_ENABLE_WARNING "Turn on compiler warnings." ON)
cmake_dependent_option(
    smoldb_cpp_WARNING_AS_ERR "Turn compiler warnings into errors" OFF
    "smoldb_cpp_ENABLE_WARNING" OFF)

#
# Setup for this project only
#

option(smoldb_cpp_ENABLE_COVERAGE "Add coverage flags" OFF)
option(smoldb_cpp_ENABLE_PCH "Use precompiled headers" OFF)
option(smoldb_cpp_ENABLE_DOXYGEN "Use Doxygen to generate documents" OFF)
# in my experience, turning this off gives better debug information
cmake_dependent_option(
    smoldb_cpp_ENABLE_DEBUG_OPTIMIZATION
    "Turn this off to disable optimization (if any) in Debug builds" ON
    "\"${CMAKE_BUILD_TYPE}\" STREQUAL \"Debug\"" ON)
option(smoldb_cpp_ENABLE_ASAN "Link libASan to executable" OFF)
option(smoldb_cpp_ENABLE_UBSAN "Link libUBSan to executable" OFF)
option(smoldb_cpp_ENABLE_MSAN "Link libMSan to executable" OFF)
option(smoldb_cpp_ENABLE_TSAN "Link libTSan to executable" OFF)
cmake_dependent_option(smoldb_cpp_USE_LIBCXX "Use libc++ instead" OFF
                       "\"${CMAKE_CXX_COMPILER_ID}\" MATCHES \".*Clang\"" OFF)

# options to include more subdirs
option(smoldb_cpp_ENABLE_UNIT_TEST
       "Build unit test executable. Requires GoogleTest." OFF)
option(smoldb_cpp_ENABLE_FUZZ_TEST
       "Build fuzz test executable. Requires libFuzzer." OFF)
option(smoldb_cpp_ENABLE_HARDENING "Add hardening flags to the compiler" OFF)
option(
    smoldb_cpp_ENABLE_BENCHMARK
    "Build micro-benchmark executable.
    Requires GoogleTest to be installed alongside Google Benchmark."
    OFF)

# installing and packing
cmake_dependent_option(smoldb_cpp_INSTALL
                    "Configure installation for this project"
                       OFF "PROJECT_IS_TOP_LEVEL" OFF)
cmake_dependent_option(smoldb_cpp_PACK "Configure packing for this project" OFF
                       "PROJECT_IS_TOP_LEVEL" OFF)

if(NOT PROJECT_IS_TOP_LEVEL)
    message(STATUS "Project is not top-level.")
    mark_as_advanced(
        smoldb_cpp_ENABLE_CCACHE
        smoldb_cpp_ENABLE_LLD
        smoldb_cpp_ENABLE_LTO
        smoldb_cpp_ENABLE_MODULE
        smoldb_cpp_ENABLE_WARNING
        smoldb_cpp_WARNING_AS_ERR
        smoldb_cpp_ENABLE_UNIT_TEST
        smoldb_cpp_ENABLE_FUZZ_TEST
        smoldb_cpp_ENABLE_BENCHMARK
        smoldb_cpp_ENABLE_OPTIMIZATION
        smoldb_cpp_ENABLE_COVERAGE
        smoldb_cpp_ENABLE_PCH
        smoldb_cpp_ENABLE_DOXYGEN
        smoldb_cpp_ENABLE_ASAN
        smoldb_cpp_ENABLE_UBSAN
        smoldb_cpp_ENABLE_MSAN
        smoldb_cpp_ENABLE_TSAN
        smoldb_cpp_INSTALL
        smoldb_cpp_PACK)
else()
    message(STATUS "Project is top-level. Configuring global options.")
    include(cmake/GlobalConfig.cmake)
    smoldb_cpp_global_config()
endif()

include(cmake/ProjectConfig.cmake)
message(STATUS "Configuring project-specific options.")
if(smoldb_cpp_ENABLE_PCH)
    set_pch_options(PCH <memory> <string> <filesystem> <expected> <cstdint>)
endif()
smoldb_cpp_local_config()
