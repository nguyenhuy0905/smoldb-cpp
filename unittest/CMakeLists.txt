# CMake's default GTest finding
include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(myproj_unittest unit-test.cxx)

target_link_libraries(myproj_unittest PRIVATE myproj_lib GTest::gtest_main)
gtest_discover_tests(myproj_unittest)
message(STATUS "After building, run ${CMAKE_CURRENT_BINARY_DIR}/myproj_unittest")
