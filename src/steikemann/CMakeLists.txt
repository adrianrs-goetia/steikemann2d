include(cmake/compile_flags.cmake)
include(cmake/generate_gdextension.cmake)
include(cmake/libname.cmake)
include(cmake/source_files.cmake)

set_libname()
generate_gdextension()

add_library(${LIBNAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${LIBNAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_dependencies(${LIBNAME}
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
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/steikemann2d/lib"
    OUTPUT_NAME ${LIBNAME}
)

install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/steikemann2d
    DESTINATION ${CMAKE_SOURCE_DIR}/project/addons/
)
