# ##############################################################################
# Project option configurations
# ##############################################################################

include(CheckCXXSourceCompiles)

# set PCH headers when PCH is enabled
macro(set_pch_options)
    cmake_parse_arguments(myproj "" "" PCH ${ARGN})
endmacro()

# local configs
macro(myproj_local_config)
    if(myproj_ENABLE_PCH)
        target_precompile_headers(myproj_compile_opts INTERFACE ${myproj_PCH})
    endif()

    if(NOT myproj_ENABLE_OPTIMIZATION)
        if(MSVC)
            target_compile_options(myproj_compile_opts INTERFACE "/Od")
        else()
            target_compile_options(myproj_compile_opts INTERFACE "-O0")
        endif()
    endif()

    if(myproj_ENABLE_COVERAGE)
        if(MSVC)
            target_compile_options(myproj_compile_opts
                                   INTERFACE "/fsanitize-coverage")
        else()
            if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
                target_compile_options(myproj_compile_opts
                                       INTERFACE "--coverage;-ftest-coverage")
                target_link_libraries(
                    myproj_compile_opts
                    INTERFACE "-fprofile-arcs;-ftest-coverage")
            elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL ".*Clang")
                target_compile_options(
                    myproj_compile_opts
                    INTERFACE
                        "-fprofile-instr-generate;-fcoverage-mapping;-mllvm;-runtime-counter-relocation"
                )
                target_link_libraries(
                    myproj_compile_opts
                    INTERFACE
                        "-fprofile-instr-generate;-fcoverage-mapping;-mllvm;-runtime-counter-relocation"
                )
            endif()
        endif()
    endif()

    include(cmake/CheckSanitizerSourceCompile.cmake)
    if(myproj_ENABLE_ASAN
       OR myproj_ENABLE_UBSAN
       OR myproj_ENABLE_MSAN
       OR myproj_ENABLE_TSAN)
        message(
            STATUS
                "Running checks on whether ASan, UBSan, MSan or TSan can be linked"
        )
        myproj_check_san_compile(myproj_ASAN_COMPILE myproj_UBSAN_COMPILE
                                 myproj_MSAN_COMPILE myproj_TSAN_COMPILE)
    endif()

    if(myproj_ENABLE_ASAN AND proj_ASAN_COMPILE)
        if(MSVC)
            target_compile_options(myproj_compile_opts
                                   INTERFACE "/fsanitize=address")
            target_compile_definitions(
                myproj_compile_opts
                INTERFACE
                    "/D_DISABLE_STRING_ANNOTATION;/D_DISABLE_VECTOR_ANNOTATION")
            target_link_libraries(myproj_compile_opts
                                  INTERFACE "/fsanitize=address")
        else()
            target_compile_options(
                myproj_compile_opts
                INTERFACE
                    "-fsanitize=address;-fno-omit-frame-pointer;-fno-optimize-sibling-calls"
            )
            target_link_libraries(myproj_compile_opts
                                  INTERFACE "-fsanitize=address")
        endif()
    endif()

    if(myproj_ENABLE_UBSAN AND myproj_UBSAN_COMPILE)
        target_compile_options(myproj_compile_opts
                               INTERFACE "-fsanitize=undefined")
        target_link_libraries(myproj_compile_opts
                              INTERFACE "-fsanitize=undefined")
    endif()

    if(myproj_ENABLE_MSAN AND myproj_MSAN_COMPILE)
        target_compile_options(
            myproj_compile_opts
            INTERFACE
                "-fsanitize=memory;-fno-omit-frame-pointer;-fno-optimize-sibling-calls"
        )
        target_link_libraries(myproj_compile_opts INTERFACE "-fsanitize=memory")
    endif()

    if(myproj_ENABLE_TSAN AND myproj_TSAN_COMPILE)
        target_compile_options(myproj_compile_opts
                               INTERFACE "-fsanitize=thread")
        target_link_libraries(myproj_compile_opts INTERFACE "-fsanitize=thread")
    endif()

    if(myproj_USE_LIBCXX)
        include(cmake/CheckLibcxxSourceCompile.cmake)
        myproj_check_libcxx_compile(myproj_LIBCXX_COMPILE)
        if(myproj_LIBCXX_COMPILE)
            target_compile_options(myproj_compile_opts
                                   INTERFACE "-stdlib=libc++")
            target_link_libraries(myproj_compile_opts
                                  INTERFACE "-stdlib=libc++")
        endif()
    endif()

endmacro()
