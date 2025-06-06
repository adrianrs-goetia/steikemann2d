include(cmake/compile_flags.cmake)
include(cmake/libname.cmake)
include(cmake/source_files.cmake)

set_libname()

add_library(${LIBNAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${LIBNAME} PRIVATE
    src

    godot-cpp/build-gcc/gen/include
    godot-cpp/gdextension
    godot-cpp/include
)

set(GODOT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp/build-gcc/bin/libgodot-cpp.linux.debug.64.a)
if (MINGW)
    set(GODOT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp/build-windows/bin/libgodot-cpp.windows.debug.64.a)
endif()

target_link_libraries(
    ${LIBNAME}
    PRIVATE
    ${GODOT_LIB}
)

set_steikemann_compile_flags(${LIBNAME})
