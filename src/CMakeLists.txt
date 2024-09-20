include(../cmake/ModuleIfEnabled.cmake)
target_include_directories(smoldb_cpp_compile_opts
    INTERFACE
    $<BUILD_INTERFACE:"${CMAKE_CURRENT_BINARY_DIR}">
    $<INSTALL_INTERFACE:"${CMAKE_INSTALL_INCLUDEDIR}">
)

smoldb_cpp_target_module_if_enabled(
    TARGET
    smoldb_cpp_data_type
    MODULE_FILES
    data_type.cxx
    HEADER_FILES
    data_type.hxx
    SOURCE_FILES
    dummy.cxx
)

smoldb_cpp_target_module_if_enabled(
    TARGET
    smoldb_cpp_lib
    MODULE_FILES
    lib.cxx
    HEADER_FILES
    dummy.hxx
    SOURCE_FILES
    lib.cxx
)

target_link_libraries(smoldb_cpp_lib
    PRIVATE
    smoldb_cpp_compile_opts
    smoldb_cpp_data_type
)

include(GenerateExportHeader)
generate_export_header(smoldb_cpp_lib BASE_NAME "smoldb_cpp")

add_library(smoldb_cpp_main)
target_sources(smoldb_cpp_main
    PRIVATE
    main.cxx)
target_link_libraries(smoldb_cpp_main
    PRIVATE
    smoldb_cpp_compile_opts)
