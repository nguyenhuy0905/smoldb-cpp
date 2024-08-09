add_subdirectory(interpreter)
add_subdirectory(general)

add_executable(smoldb_main main.cxx)

target_link_libraries(smoldb_main PRIVATE smoldb_compile_opts smoldb_interpreter smoldb_debug)
# target_include_directories(
#     smoldb_lib INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
