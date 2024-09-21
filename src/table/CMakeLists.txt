smoldb_cpp_target_module_if_enabled(
    TARGET
    smoldb_cpp_column_meta
    MODULE_FILES
    column_meta.cxxm
    HEADER_FILES
    column_meta.hxx
    SOURCE_FILES
    column_meta.cxx
)

target_link_libraries(smoldb_cpp_column_meta
    PRIVATE
    smoldb_cpp_compile_opts
)

smoldb_cpp_target_module_if_enabled(
    TARGET
    smoldb_cpp_table
    MODULE_FILES
    table.cxxm
    HEADER_FILES
    table.hxx
    SOURCE_FILES
    table.cxx
)

target_link_libraries(smoldb_cpp_table
    PRIVATE
    smoldb_cpp_compile_opts
    PUBLIC
    smoldb_cpp_column_meta
)
