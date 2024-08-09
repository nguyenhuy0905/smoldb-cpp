add_library(smoldb_interpreter interpreter.cpp)
target_link_libraries(smoldb_interpreter PRIVATE smoldb_compile_opts smoldb_debug)
target_sources(smoldb_interpreter PUBLIC FILE_SET CXX_MODULES FILES
                                         interpreter.cppm)
