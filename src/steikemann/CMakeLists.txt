include(cmake/compile_flags.cmake)

set(HEADERS
)
set(SOURCES
    register_types.cpp
)

include(cmake/libname.cmake)

add_library(${LIBNAME} SHARED ${HEADERS} ${SOURCES})
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
    # ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/steikemann2d.gdextension 
    DESTINATION ${CMAKE_SOURCE_DIR}/project/addons/steikemann2d/
)
