add_executable(myproj_fuzztest fuzz-test.cxx)

target_compile_options(myproj_fuzztest PRIVATE "-fsanitize=fuzzer")
target_link_libraries(myproj_fuzztest PRIVATE myproj_compile_opts myproj_lib -fsanitize=fuzzer)

message("After building, run ${CMAKE_CURRENT_BINARY_DIR}/myproj_fuzztest")
