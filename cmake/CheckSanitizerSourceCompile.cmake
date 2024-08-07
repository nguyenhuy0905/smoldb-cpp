# ##############################################################################
# Sanitizer compat check
# ##############################################################################

# check if ASan can be linked
function(myproj_check_san_compile asan_output_var ubsan_output_var
         msan_output_var tsan_output_var)
    set(test_code
        "
    int main() {
        return 0;
    }
    ")
    if(MSVC)
        set(CMAKE_REQUIRED_DEFINITIONS
            "-D_DISABLE_VECTOR_ANNOTATION;-D_DISABLE_STRING_ANNOTATION")
        set(CMAKE_REQUIRED_FLAGS "/fsanitize=address")
        set(CMAKE_REQUIRED_LIBRARIES "/fsanitize=address")
    else()
        set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
        set(CMAKE_REQUIRED_LIBRARIES "-fsanitize=address")
    endif()
    check_cxx_source_compiles("${test_code}" ${asan_output_var})

    if(MSVC)
        message(WARNING "UBSan is not supported on MSVC")
        set(${ubsan_output_var} OFF)
    else()
        set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
        set(CMAKE_REQUIRED_LIBRARIES "-fsanitize=undefined")
        check_cxx_source_compiles("${test_code}" ${ubsan_output_var})
    endif()

    if(MSVC)
        message(WARNING "MSan is not supported on MSVC")
        set(${ubsan_output_var} OFF)
    else()
        set(CMAKE_REQUIRED_FLAGS "-fsanitize=memory")
        set(CMAKE_REQUIRED_LIBRARIES "-fsanitize=memory")
        check_cxx_source_compiles("${test_code}" ${msan_output_var})
    endif()

    if(MSVC)
        message(WARNING "TSan is not supported on MSVC")
        set(${ubsan_output_var} OFF)
    else()
        set(CMAKE_REQUIRED_FLAGS "-fsanitize=memory")
        set(CMAKE_REQUIRED_LIBRARIES "-fsanitize=memory")
        check_cxx_source_compiles("${test_code}" ${tsan_output_var})
    endif()

endfunction()
