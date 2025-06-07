include(cmake/compiler_warnings.cmake)
include(cmake/compile_flags.cmake)
include(cmake/libname.cmake)
include(cmake/source_files.cmake)

add_subdirectory(godot-cpp)

set_libname()

add_library(${LIBNAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${LIBNAME} PRIVATE src)

target_link_libraries(
    ${LIBNAME}
    PRIVATE
    godot-cpp
)

set_steikemann_compile_flags(${LIBNAME})
