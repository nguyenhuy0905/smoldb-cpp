# ##############################################################################
# Project option configurations
# ##############################################################################

include(CheckCXXSourceCompiles)

# set PCH headers when PCH is enabled
macro(set_pch_options)
    cmake_parse_arguments(smoldb "" "" PCH ${ARGN})
endmacro()

# local configs
macro(smoldb_local_config)
    if(smoldb_ENABLE_PCH)
        target_precompile_headers(smoldb_compile_opts INTERFACE ${smoldb_PCH})
    endif()

    if(NOT smoldb_ENABLE_OPTIMIZATION)
        if(MSVC)
            target_compile_options(smoldb_compile_opts INTERFACE "/Od")
        else()
            target_compile_options(smoldb_compile_opts INTERFACE "-O0")
        endif()
    endif()

    if(smoldb_ENABLE_COVERAGE)
        if(MSVC)
            target_compile_options(smoldb_compile_opts
                                   INTERFACE "/fsanitize-coverage")
        else()
            if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
                target_compile_options(smoldb_compile_opts
                                       INTERFACE "--coverage;-ftest-coverage")
                target_link_libraries(
                    smoldb_compile_opts
                    INTERFACE "-fprofile-arcs;-ftest-coverage")
            elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL ".*Clang")
                target_compile_options(
                    smoldb_compile_opts
                    INTERFACE
                        "-fprofile-instr-generate;-fcoverage-mapping;-mllvm;-runtime-counter-relocation"
                )
                target_link_libraries(
                    smoldb_compile_opts
                    INTERFACE
                        "-fprofile-instr-generate;-fcoverage-mapping;-mllvm;-runtime-counter-relocation"
                )
            endif()
        endif()
    endif()

    include(cmake/CheckSanitizerSourceCompile.cmake)
    if(smoldb_ENABLE_ASAN
       OR smoldb_ENABLE_UBSAN
       OR smoldb_ENABLE_MSAN
       OR smoldb_ENABLE_TSAN)
        message(
            STATUS
                "Running checks on whether ASan, UBSan, MSan or TSan can be linked"
        )
        smoldb_check_san_compile(smoldb_ASAN_COMPILE smoldb_UBSAN_COMPILE
                                 smoldb_MSAN_COMPILE smoldb_TSAN_COMPILE)
    endif()

    if(smoldb_ENABLE_ASAN AND proj_ASAN_COMPILE)
        if(MSVC)
            target_compile_options(smoldb_compile_opts
                                   INTERFACE "/fsanitize=address")
            target_compile_definitions(
                smoldb_compile_opts
                INTERFACE
                    "/D_DISABLE_STRING_ANNOTATION;/D_DISABLE_VECTOR_ANNOTATION")
            target_link_libraries(smoldb_compile_opts
                                  INTERFACE "/fsanitize=address")
        else()
            target_compile_options(
                smoldb_compile_opts
                INTERFACE
                    "-fsanitize=address;-fno-omit-frame-pointer;-fno-optimize-sibling-calls"
            )
            target_link_libraries(smoldb_compile_opts
                                  INTERFACE "-fsanitize=address")
        endif()
    endif()

    if(smoldb_ENABLE_UBSAN AND smoldb_UBSAN_COMPILE)
        target_compile_options(smoldb_compile_opts
                               INTERFACE "-fsanitize=undefined")
        target_link_libraries(smoldb_compile_opts
                              INTERFACE "-fsanitize=undefined")
    endif()

    if(smoldb_ENABLE_MSAN AND smoldb_MSAN_COMPILE)
        target_compile_options(
            smoldb_compile_opts
            INTERFACE
                "-fsanitize=memory;-fno-omit-frame-pointer;-fno-optimize-sibling-calls"
        )
        target_link_libraries(smoldb_compile_opts INTERFACE "-fsanitize=memory")
    endif()

    if(smoldb_ENABLE_TSAN AND smoldb_TSAN_COMPILE)
        target_compile_options(smoldb_compile_opts
                               INTERFACE "-fsanitize=thread")
        target_link_libraries(smoldb_compile_opts INTERFACE "-fsanitize=thread")
    endif()

    if(smoldb_USE_LIBCXX)
        include(cmake/CheckLibcxxSourceCompile.cmake)
        smoldb_check_libcxx_compile(smoldb_LIBCXX_COMPILE)
        if(smoldb_LIBCXX_COMPILE)
            target_compile_options(smoldb_compile_opts
                                   INTERFACE "-stdlib=libc++")
            target_link_libraries(smoldb_compile_opts
                                  INTERFACE "-stdlib=libc++")
        endif()
    endif()

endmacro()
