# ##############################################################################
# List of options
# ##############################################################################

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

# ##############################################################################
# Global options
# ##############################################################################

# If there's another top-level project, high chance that project already sets
# these options.
option(smoldb_ENABLE_CCACHE "Use ccache" OFF)
option(smoldb_ENABLE_LLD "Use lld instead of the compiler-default linker" OFF)
option(smoldb_ENABLE_LTO "Enable link-time optimization" OFF)
option(smoldb_ENABLE_WARNING "Turn on compiler warnings." ON)
cmake_dependent_option(
    smoldb_WARNING_AS_ERR "Turn compiler warnings into errors" OFF
    "smoldb_ENABLE_WARNING" OFF)

# ##############################################################################
# Setup for this project only
# ##############################################################################

option(smoldb_ENABLE_COVERAGE "Add coverage flags" OFF)
option(smoldb_ENABLE_PCH "Use precompiled headers" OFF)
# in my experience, turning this off gives better debug information
cmake_dependent_option(
    smoldb_ENABLE_DEBUG_OPTIMIZATION
    "Turn this off to disable optimization (if any) in Debug builds" ON
    "\"${CMAKE_BUILD_TYPE}\" STREQUAL \"Debug\"" ON)
option(smoldb_ENABLE_ASAN "Link libASan to executable" OFF)
option(smoldb_ENABLE_UBSAN "Link libUBSan to executable" OFF)
option(smoldb_ENABLE_MSAN "Link libMSan to executable" OFF)
option(smoldb_ENABLE_TSAN "Link libTSan to executable" OFF)
cmake_dependent_option(smoldb_USE_LIBCXX "Use libc++ instead" OFF
                       "\"${CMAKE_CXX_COMPILER_ID}\" MATCHES \".*Clang\"" OFF)

# options to include more subdirs
option(smoldb_ENABLE_UNIT_TEST "Build unit test executable" OFF)
option(smoldb_ENABLE_FUZZ_TEST "Build fuzz test executable" OFF)

# installing and packing
cmake_dependent_option(smoldb_INSTALL "Configure installation for this project"
                       OFF "PROJECT_IS_TOP_LEVEL" OFF)
cmake_dependent_option(smoldb_PACK "Configure packing for this project" OFF
                       "PROJECT_IS_TOP_LEVEL" OFF)

if(NOT PROJECT_IS_TOP_LEVEL)
    message(STATUS "Project is not top-level.")
    mark_as_advanced(
        smoldb_ENABLE_CCACHE
        smoldb_ENABLE_LLD
        smoldb_ENABLE_LTO
        smoldb_ENABLE_WARNING
        smoldb_WARNING_AS_ERR
        smoldb_ENABLE_UNIT_TEST
        smoldb_ENABLE_FUZZ_TEST
        smoldb_ENABLE_OPTIMIZATION
        smoldb_ENABLE_COVERAGE
        smoldb_ENABLE_PCH
        smoldb_ENABLE_ASAN
        smoldb_ENABLE_UBSAN
        smoldb_ENABLE_MSAN
        smoldb_ENABLE_TSAN
        smoldb_INSTALL
        smoldb_PACK)
else()
    message(STATUS "Project is top-level. Configuring global options.")
    include(cmake/GlobalConfig.cmake)
    smoldb_global_config()
endif()

include(cmake/ProjectConfig.cmake)
message(STATUS "Configuring project-specific options.")
if(smoldb_ENABLE_PCH)
    set_pch_options(PCH <memory> <string> <string_view>)
endif()
smoldb_local_config()
