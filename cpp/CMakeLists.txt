include(cmake/compiler_warnings.cmake)
include(cmake/compile_flags.cmake)
include(cmake/libname.cmake)
include(cmake/source_files.cmake)

add_subdirectory(godot-cpp)

set_libname()

message(STATUS "message text ${SOURCES}")
add_library(${LIBNAME} SHARED ${SOURCES})

target_include_directories(${LIBNAME} PRIVATE
    src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${LIBNAME}
    PRIVATE
    godot-cpp
)

set_steikemann_compile_flags(${LIBNAME})


set_target_properties(${LIBNAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/project/addons/steikemann2d/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/project/addons/steikemann2d/lib"
)
