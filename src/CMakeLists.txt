add_executable(myproj_main main.cxx)
add_library(myproj_lib lib.cxx)

target_link_libraries(myproj_main PRIVATE myproj_compile_opts)
target_include_directories(
    myproj_lib INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
