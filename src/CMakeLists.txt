include(../cmake/ModuleIfEnabled.cmake)

add_executable(myproj_main main.cxx)
myproj_target_module_if_enabled(
    TARGET
    myproj_lib
    MODULE_FILES
    lib.cxx
    HEADER_FILES
    lib.hxx
    SOURCE_FILES
    lib.cxx)

include(GenerateExportHeader)
generate_export_header(myproj_lib BASE_NAME "myproj")

add_library(myproj_system_header INTERFACE)

target_include_directories(
    myproj_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>
                      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(myproj_main PRIVATE myproj_compile_opts myproj_lib)
