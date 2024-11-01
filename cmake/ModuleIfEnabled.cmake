# Target source with module if enabled, otherwise, target with header and use
# the module sources as items passed into add_library. The targets headers or
# modules will be public. The target source files will be private.
#
# If module is not enabled, MODULE_FILES will be privately targeted as TARGET's
# source file.
#
# TARGET: should NOT be declared prior to the function call. Will be a library.
#
# MODULE_FILES: files to be targeted as CXX_MODULES in target_sources
#
# HEADER_FILES: files to be targeted as HEADERS in target_sources
#
# SOURCE_FILES: files to be passed as source files
function(smoldb_cpp_target_module_if_enabled)
    cmake_parse_arguments(smoldb_cpp "" "TARGET"
                          "MODULE_FILES;SOURCE_FILES;HEADER_FILES" ${ARGN})
    if(smoldb_cpp_UNPARSED_ARGUMENTS)
        message(
            FATAL_ERROR
                "Unknown arguments passed in: ${smoldb_cpp_UNPARSED_ARGUMENTS}")
    endif()
    target_link_libraries(${smoldb_cpp_TARGET} PRIVATE smoldb_cpp_compile_opts)
    if(smoldb_cpp_HEADER_FILES)
        target_sources(
        ${smoldb_cpp_TARGET}
        PUBLIC FILE_SET HEADERS
        FILES
        ${smoldb_cpp_HEADER_FILES}
        )
    endif()
    if(smoldb_cpp_SOURCE_FILES)
        target_sources(${smoldb_cpp_TARGET}
            PRIVATE
            ${smoldb_cpp_SOURCE_FILES}
        )
    elseif(NOT smoldb_cpp_ENABLE_MODULE)
        target_sources(${smoldb_cpp_TARGET}
            PRIVATE
            ${PROJECT_SOURCE_DIR}/cmake/dummy.cxx
        )
    endif()
    if(smoldb_cpp_ENABLE_MODULE)
        target_sources(${smoldb_cpp_TARGET}
            PUBLIC FILE_SET CXX_MODULES
            FILES
            ${smoldb_cpp_MODULE_FILES})
    endif()
endfunction()
