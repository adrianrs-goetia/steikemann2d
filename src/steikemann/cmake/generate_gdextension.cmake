macro(generate_gdextension)
    set(STEIKEMANN_GDEXTENSION_FILE "${CMAKE_BINARY_DIR}/src/steikemann/steikemann2d.gdextension")
    set(STEIKEMANN_GDEXTENSION_FILE_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/steikemann2d.gdextension.template")
    set(STEIKEMANN_GDEXTENSION_GENERATOR_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/script/generate_gdextension.cmake")

    add_custom_command(
        OUTPUT ${STEIKEMANN_GDEXTENSION_FILE}
        COMMENT "Generating steikemann2d.gdextension to build directory"

        COMMAND ${CMAKE_COMMAND} -E copy "${STEIKEMANN_GDEXTENSION_FILE_TEMPLATE}" "${CMAKE_CURRENT_BINARY_DIR}/steikemann2d.gdextension.template"
        DEPENDS ${STEIKEMANN_VERSION_INPUT_FILE}

        COMMAND ${CMAKE_COMMAND}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/scripts/generate_gdextension.cmake
        DEPENDS ${STEIKEMANN_GDEXTENSION_FILE_TEMPLATE}
    )
    add_custom_target(generate_gdextension_file ALL DEPENDS ${STEIKEMANN_GDEXTENSION_FILE})
endmacro(generate_gdextension)
