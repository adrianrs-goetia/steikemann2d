include(cmake/compiler_warnings.cmake)
include(cmake/compile_flags.cmake)
include(cmake/libname.cmake)
include(cmake/source_files.cmake)

add_subdirectory(godot-cpp)

set_libname()

message(STATUS "message text ${SOURCES}")
add_library(${LIBNAME} SHARED ${SOURCES})

target_include_directories(${LIBNAME} PRIVATE src)

target_link_libraries(
    ${LIBNAME}
    PRIVATE
    godot-cpp
)

set_steikemann_compile_flags(${LIBNAME})

if (MINGW) # Tracking .dll files in git, so we place them in the project dir
    set_target_properties(${LIBNAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/project/addons/steikemann2d/lib"
    )
endif()
