# CMake's default GTest finding
include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(smoldb_unittest unit-test.cxx)

target_link_libraries(smoldb_unittest PRIVATE smoldb_lib GTest::gtest_main)
gtest_discover_tests(smoldb_unittest)
message(STATUS "After building, run ${CMAKE_CURRENT_BINARY_DIR}/smoldb_unittest")
