add_executable(smoldb_fuzztest fuzz-test.cxx)

target_compile_options(smoldb_fuzztest PRIVATE "-fsanitize=fuzzer")
target_link_libraries(smoldb_fuzztest PRIVATE smoldb_compile_opts smoldb_lib -fsanitize=fuzzer)

message("After building, run ${CMAKE_CURRENT_BINARY_DIR}/smoldb_fuzztest")
