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
macro(myproj_target_module_if_enabled)
    cmake_parse_arguments(myproj "" "TARGET"
                          "MODULE_FILES;SOURCE_FILES;HEADER_FILES" ${ARGN})
    if(myproj_UNPARSED_ARGUMENTS)
        message(
            FATAL_ERROR
                "Unknown arguments passed in: ${myproj_UNPARSED_ARGUMENTS}")
    endif()
    add_library(${myproj_TARGET})
    target_link_libraries(${myproj_TARGET} PRIVATE myproj_compile_opts)
    target_sources(
        ${myproj_TARGET}
        PUBLIC FILE_SET HEADERS FILES ${myproj_HEADER_FILES}
        PRIVATE ${myproj_SOURCE_FILES} ${myproj_MODULE_FILES}
                ${PROJECT_SOURCE_DIR}/cmake/dummy.cxx)
    if(myproj_ENABLE_MODULE)
        target_sources(${myproj_TARGET} PUBLIC FILE_SET CXX_MODULES FILES
                                               ${myproj_MODULE_FILES})
    endif()
endmacro()
