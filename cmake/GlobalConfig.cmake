# ##############################################################################
# Global option configurations
# ##############################################################################

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
