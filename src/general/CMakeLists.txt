add_library(smoldb_debug INTERFACE)
target_link_libraries(smoldb_debug INTERFACE smoldb_compile_opts)
target_sources(smoldb_debug INTERFACE FILE_SET HEADERS FILES debug.hpp)
