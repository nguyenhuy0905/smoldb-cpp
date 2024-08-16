include(../cmake/ModuleIfEnabled.cmake)

add_executable(smoldb_cpp_main main.cxx)
smoldb_cpp_target_module_if_enabled(
    TARGET
    smoldb_cpp_lib
    MODULE_FILES
    lib.cxx
    HEADER_FILES
    lib.hxx
    SOURCE_FILES
    lib.cxx)

include(GenerateExportHeader)
generate_export_header(smoldb_cpp_lib BASE_NAME "smoldb_cpp")

add_library(smoldb_cpp_system_header INTERFACE)

target_include_directories(
    smoldb_cpp_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>
                      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(smoldb_cpp_main PRIVATE smoldb_cpp_compile_opts smoldb_cpp_lib)
