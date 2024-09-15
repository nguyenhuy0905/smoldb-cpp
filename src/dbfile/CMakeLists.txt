add_library(smoldb_cpp_dbfile)
# include current binary dir in case I ever pre-process some files.
target_sources(smoldb_cpp_dbfile PRIVATE dbfile.cxx)
target_include_directories(smoldb_cpp_dbfile INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(smoldb_cpp_dbfile PRIVATE smoldb_cpp_compile_opts)

#TODO: comment out PCH when done with dbfile headers.

# target_precompile_headers(smoldb_cpp_dbfile PUBLIC
#     dbfile.hxx
#     dbfile_builder.hxx
# )
