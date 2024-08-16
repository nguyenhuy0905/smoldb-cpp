# Project option configurations

include(CheckCXXSourceCompiles)

# set PCH headers when PCH is enabled
macro(set_pch_options)
    cmake_parse_arguments(smoldb_cpp "" "" PCH ${ARGN})
    target_precompile_headers(smoldb_cpp_compile_opts INTERFACE ${smoldb_cpp_PCH})
endmacro()

# local configs
macro(smoldb_cpp_local_config)
    if(NOT smoldb_cpp_ENABLE_DEBUG_OPTIMIZATION)
        if(MSVC)
            target_compile_options(smoldb_cpp_compile_opts INTERFACE "/Od")
        else()
            target_compile_options(smoldb_cpp_compile_opts INTERFACE "-O0")
        endif()
    else()
        if(MSVC)
            # target_compile_options(smoldb_cpp_compile_opts INTERFACE "/Og")
        else()
            target_compile_options(smoldb_cpp_compile_opts INTERFACE "-Og")
        endif()
    endif()

    if(smoldb_cpp_ENABLE_COVERAGE)
        if(MSVC)
            target_compile_options(smoldb_cpp_compile_opts
                                   INTERFACE "/fsanitize-coverage")
        else()
            target_compile_options(smoldb_cpp_compile_opts INTERFACE "--coverage")
            target_link_libraries(smoldb_cpp_compile_opts INTERFACE "--coverage")
        endif()
    endif()

    include(cmake/CheckSanitizerSourceCompile.cmake)
    if(smoldb_cpp_ENABLE_ASAN OR smoldb_cpp_ENABLE_UBSAN OR smoldb_cpp_ENABLE_MSAN OR smoldb_cpp_ENABLE_TSAN)
        message(STATUS "Running checks on whether ASan, UBSan, MSan or TSan can be linked")
        smoldb_cpp_check_san_compile(smoldb_cpp_compile_opts INTERFACE)
    endif()

    if(smoldb_cpp_ENABLE_HARDENING)
        include(cmake/AddHardeningFlags.cmake)
        smoldb_cpp_add_hardening_flags(smoldb_cpp_compile_opts INTERFACE)
    endif()

endmacro()
