add_library(smoldb_cpp_table)
target_sources(smoldb_cpp_table PRIVATE table.cxx)
target_include_directories(smoldb_cpp_table INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(smoldb_cpp_table PRIVATE smoldb_cpp_compile_opts)
