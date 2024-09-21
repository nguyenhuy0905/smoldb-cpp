include(../cmake/ModuleIfEnabled.cmake)
target_include_directories(smoldb_cpp_compile_opts
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR};${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_subdirectory(table)

add_library(smoldb_cpp_lib)
smoldb_cpp_target_module_if_enabled(
    TARGET
    smoldb_cpp_lib
    MODULE_FILES
    lib.cxxm
    SOURCE_FILES
    lib.cxxm
)

target_link_libraries(smoldb_cpp_lib
    PUBLIC
    smoldb_cpp_column_meta
    smoldb_cpp_table
    PRIVATE
    smoldb_cpp_compile_opts
)

include(GenerateExportHeader)
generate_export_header(smoldb_cpp_lib BASE_NAME "smoldb_cpp")

add_library(smoldb_cpp_main)
target_sources(smoldb_cpp_main
    PRIVATE
    main.cxx)
target_link_libraries(
    smoldb_cpp_main
    PUBLIC
    smoldb_cpp_lib
    PRIVATE
    smoldb_cpp_compile_opts)
