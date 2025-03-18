macro(generate_gdextension)
    set(STEIKEMANN_GDEXTENSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/steikemann2d/steikemann2d.gdextension")
    set(STEIKEMANN_GDEXTENSION_FILE_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/steikemann2d.gdextension.template")
    set(STEIKEMANN_GDEXTENSION_GENERATOR_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/script/generate_gdextension.cmake")

    message(STATUS "generate_gdextension version: ${STEIKEMANN_VERSION}")

    add_custom_command(
        OUTPUT ${STEIKEMANN_GDEXTENSION_FILE}
        COMMENT "Generating steikemann2d.gdextension to build directory"

        COMMAND ${CMAKE_COMMAND} -E copy "${STEIKEMANN_GDEXTENSION_FILE_TEMPLATE}" "${CMAKE_CURRENT_BINARY_DIR}/steikemann2d.gdextension.template"
        DEPENDS ${STEIKEMANN_GDEXTENSION_FILE_TEMPLATE} ${STEIKEMANN_VERSION_INPUT_FILE}

        COMMAND ${CMAKE_COMMAND}
        -DSTEIKEMANN_VERSION=${STEIKEMANN_VERSION}
        -DSTEIKEMANN_GDEXTENSION_FILE=${STEIKEMANN_GDEXTENSION_FILE}
        -DSTEIKEMANN_GDEXTENSION_FILE_TEMPLATE=${STEIKEMANN_GDEXTENSION_FILE_TEMPLATE}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/scripts/generate_gdextension.cmake
        DEPENDS ${STEIKEMANN_GDEXTENSION_FILE_TEMPLATE} ${STEIKEMANN_VERSION_INPUT_FILE}
    )
    add_custom_target(generate_gdextension_file ALL DEPENDS ${STEIKEMANN_GDEXTENSION_FILE})
endmacro(generate_gdextension)
