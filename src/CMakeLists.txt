include(../cmake/ModuleIfEnabled.cmake)
add_subdirectory(dbfile)
add_subdirectory(table)
add_subdirectory(data_type)

add_executable(smoldb_cpp_main)
target_sources(smoldb_cpp_main PRIVATE main.cxx)
target_link_libraries(smoldb_cpp_main PRIVATE
    smoldb_cpp_compile_opts
    smoldb_cpp_dbfile
    smoldb_cpp_table)
