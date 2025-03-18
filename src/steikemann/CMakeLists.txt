include(cmake/compile_flags.cmake)
include(cmake/copy_version_file.cmake)
include(cmake/generate_gdextension.cmake)
include(cmake/generate_version_h.cmake)
include(cmake/libname.cmake)
include(cmake/source_files.cmake)

generate_version_h()
set_libname()
generate_gdextension()

add_library(${LIBNAME} SHARED ${HEADERS} ${SOURCES})

target_sources(${LIBNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h PROPERTIES HEADER_FILE_ONLY TRUE)

target_include_directories(${LIBNAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_dependencies(${LIBNAME}
    copy_version_file
    generate_gdextension_file
)

target_link_libraries(
    ${LIBNAME}
    PRIVATE
    godot::cpp
)

set_steikemann_compile_flags(${LIBNAME})

set_target_properties(${LIBNAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME ${LIBNAME}
)

install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/steikemann2d.gdextension
    DESTINATION ${CMAKE_SOURCE_DIR}/project/addons/steikemann2d/
)
