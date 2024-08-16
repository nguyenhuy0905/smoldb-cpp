# Project option configurations

include(CheckCXXSourceCompiles)

# set PCH headers when PCH is enabled
macro(set_pch_options)
    cmake_parse_arguments(myproj "" "" PCH ${ARGN})
    target_precompile_headers(myproj_compile_opts INTERFACE ${myproj_PCH})
endmacro()

# local configs
macro(myproj_local_config)
    if(NOT myproj_ENABLE_DEBUG_OPTIMIZATION)
        if(MSVC)
            target_compile_options(myproj_compile_opts INTERFACE "/Od")
        else()
            target_compile_options(myproj_compile_opts INTERFACE "-O0")
        endif()
    else()
        if(MSVC)
            # target_compile_options(myproj_compile_opts INTERFACE "/Og")
        else()
            target_compile_options(myproj_compile_opts INTERFACE "-Og")
        endif()
    endif()

    if(myproj_ENABLE_COVERAGE)
        if(MSVC)
            target_compile_options(myproj_compile_opts
                                   INTERFACE "/fsanitize-coverage")
        else()
            target_compile_options(myproj_compile_opts INTERFACE "--coverage")
            target_link_libraries(myproj_compile_opts INTERFACE "--coverage")
        endif()
    endif()

    include(cmake/CheckSanitizerSourceCompile.cmake)
    if(myproj_ENABLE_ASAN OR myproj_ENABLE_UBSAN OR myproj_ENABLE_MSAN OR myproj_ENABLE_TSAN)
        message(STATUS "Running checks on whether ASan, UBSan, MSan or TSan can be linked")
        myproj_check_san_compile(myproj_compile_opts INTERFACE)
    endif()

    if(myproj_ENABLE_HARDENING)
        include(cmake/AddHardeningFlags.cmake)
        myproj_add_hardening_flags(myproj_compile_opts INTERFACE)
    endif()

endmacro()
