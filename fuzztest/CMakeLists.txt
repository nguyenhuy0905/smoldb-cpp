add_executable(smoldb_cpp_fuzztest fuzz-test.cxx)

target_compile_options(smoldb_cpp_fuzztest PRIVATE "-fsanitize=fuzzer")
target_link_libraries(smoldb_cpp_fuzztest PRIVATE smoldb_cpp_compile_opts smoldb_cpp_lib -fsanitize=fuzzer)

message("After building, run ${CMAKE_CURRENT_BINARY_DIR}/smoldb_cpp_fuzztest")
