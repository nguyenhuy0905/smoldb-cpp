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
option(myproj_ENABLE_CCACHE "Use ccache" OFF)
option(myproj_ENABLE_LLD "Use lld instead of the compiler-default linker" OFF)
option(myproj_ENABLE_LTO "Enable link-time optimization" OFF)
option(myproj_ENABLE_WARNING "Turn on compiler warnings." ON)
cmake_dependent_option(
    myproj_WARNING_AS_ERR "Turn compiler warnings into errors" OFF
    "myproj_ENABLE_WARNING" OFF)

# ##############################################################################
# Setup for this project only
# ##############################################################################

option(myproj_ENABLE_COVERAGE "Add coverage flags" OFF)
option(myproj_ENABLE_PCH "Use precompiled headers" OFF)
# in my experience, turning this off gives better debug information
cmake_dependent_option(
    myproj_ENABLE_DEBUG_OPTIMIZATION
    "Turn this off to disable optimization (if any) in Debug builds" ON
    "\"${CMAKE_BUILD_TYPE}\" STREQUAL \"Debug\"" ON)
option(myproj_ENABLE_ASAN "Link libASan to executable" OFF)
option(myproj_ENABLE_UBSAN "Link libUBSan to executable" OFF)
option(myproj_ENABLE_MSAN "Link libMSan to executable" OFF)
option(myproj_ENABLE_TSAN "Link libTSan to executable" OFF)
cmake_dependent_option(myproj_USE_LIBCXX "Use libc++ instead" OFF
                       "\"${CMAKE_CXX_COMPILER_ID}\" MATCHES \".*Clang\"" OFF)

# options to include more subdirs
option(myproj_ENABLE_UNIT_TEST "Build unit test executable" OFF)
option(myproj_ENABLE_FUZZ_TEST "Build fuzz test executable" OFF)

# installing and packing
cmake_dependent_option(myproj_INSTALL "Configure installation for this project"
                       OFF "PROJECT_IS_TOP_LEVEL" OFF)
cmake_dependent_option(myproj_PACK "Configure packing for this project" OFF
                       "PROJECT_IS_TOP_LEVEL" OFF)

if(NOT PROJECT_IS_TOP_LEVEL)
    message(STATUS "Project is not top-level.")
    mark_as_advanced(
        myproj_ENABLE_CCACHE
        myproj_ENABLE_LLD
        myproj_ENABLE_LTO
        myproj_ENABLE_WARNING
        myproj_WARNING_AS_ERR
        myproj_ENABLE_UNIT_TEST
        myproj_ENABLE_FUZZ_TEST
        myproj_ENABLE_OPTIMIZATION
        myproj_ENABLE_COVERAGE
        myproj_ENABLE_PCH
        myproj_ENABLE_ASAN
        myproj_ENABLE_UBSAN
        myproj_ENABLE_MSAN
        myproj_ENABLE_TSAN
        myproj_INSTALL
        myproj_PACK)
else()
    message(STATUS "Project is top-level. Configuring global options.")
    include(cmake/GlobalConfig.cmake)
    myproj_global_config()
endif()

include(cmake/ProjectConfig.cmake)
message(STATUS "Configuring project-specific options.")
if(myproj_ENABLE_PCH)
    set_pch_options(PCH <memory> <string>)
endif()
myproj_local_config()
