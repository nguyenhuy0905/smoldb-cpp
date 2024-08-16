find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)
add_executable(smoldb_cpp_benchmark benchmark.cxx)
target_link_libraries(smoldb_cpp_benchmark PRIVATE smoldb_cpp_compile_opts
                                               benchmark::benchmark_main GTest::gtest)
