find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)
add_executable(myproj_benchmark benchmark.cxx)
target_link_libraries(myproj_benchmark PRIVATE myproj_compile_opts
                                               benchmark::benchmark_main GTest::gtest)
