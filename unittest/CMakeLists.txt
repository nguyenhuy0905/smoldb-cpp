# CMake's default GTest finding
enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(smoldb_cpp_unittest unit-test.cxx)

target_link_libraries(smoldb_cpp_unittest PRIVATE
    smoldb_cpp_dbfile GTest::gtest_main)
gtest_discover_tests(smoldb_cpp_unittest)
message(STATUS
    "After building, run ${CMAKE_CURRENT_BINARY_DIR}/smoldb_cpp_unittest")
