# ##############################################################################
# Global option configurations
# ##############################################################################

include(CheckCXXSourceCompiles)

# global configurations
macro(myproj_global_config)
    # ccache
    if(myproj_ENABLE_CCACHE)
        message(STATUS "Configured to use ccache.")
        find_program(CCACHE ccache)
        if(NOT CCACHE)
            message(FATAL_ERROR "Could not find ccache. Aborting.")
        else()
            message(STATUS "Found ccache and is using it")
            set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
            set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
        endif()
    endif()

    # lld
    if(myproj_ENABLE_LLD)
        message(STATUS "Configured to use lld.")
        find_program(LLD lld)
        if(NOT LLD)
            message(FATAL_ERROR "Could not find lld. Aborting.")
        else()
            if(MSVC)
                add_link_options("/link;/lld")
            else()
                add_link_options("-fuse-ld=lld")
            endif()
        endif()
    endif()

    # IPO or LTO
    if(myproj_ENABLE_LTO)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif()

    # warnings
    if(myproj_ENABLE_WARNING)
        message(STATUS "Turning on more warnings")
        if(MSVC)
            add_compile_options("/W4")
        else(MSVC)
            add_compile_options(
                "-Wall;-Wextra;-Wshadow;-Wformat=2;-Wconversion")
        endif()
    endif()

    if(myproj_ENABLE_MODULE)
        if(MSVC)
            add_compile_definitions("/DENABLE_MODULE")
        else()
            add_compile_definitions("-DENABLE_MODULE")
        endif()
    endif()

    # libc++
    if(myproj_USE_LIBCXX)
        include(cmake/CheckLibcxxSourceCompile.cmake)
        myproj_check_libcxx_compile(myproj_LIBCXX_COMPILE)
        if(myproj_LIBCXX_COMPILE)
            set(CMAKE_CXX_LINKER_FLAGS
                "${CMAKE_CXX_LINKER_FLAGS} -stdlib=libc++;-lc++abi")
        endif()
    endif()

    # warning as error
    if(myproj_WARNING_AS_ERR)
        message(STATUS "Turn on warning-as-error")
        if(MSVC)
            add_compile_options("/WX")
        else(MSVC)
            add_compile_options("-Werror")
        endif()
    endif()
endmacro()
