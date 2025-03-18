macro(generate_version_h)
    set(STEIKEMANN_VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h")
    set(STEIKEMANN_VERSION_INSTALL_FILE "${CMAKE_SOURCE_DIR}/install/version.h")

    # Read from version.txt
    file(READ "${STEIKEMANN_VERSION_FILE}" contents)
    string(REPLACE "\n" ";" contents "${contents}")

    foreach(line IN LISTS contents)
        string(REGEX MATCH "([A-Z]+)[ \t]+([0-9]+)" _ "${line}")
        set(key "${CMAKE_MATCH_1}")
        set(value "${CMAKE_MATCH_2}")
        set(VERSION_${key} ${value})
    endforeach()

    set(STEIKEMANN_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

    # # Add command to copy file into binary dir
    # add_custom_command(
    #     OUTPUT ${STEIKEMANN_VERSION_FILE}
    #     COMMENT "Copying src/version.h to build directory"

    #     COMMAND ${CMAKE_COMMAND} -E copy ${STEIKEMANN_VERSION_FILE} ${STEIKEMANN_VERSION_INSTALL_FILE}
    # )
    # add_custom_target(generate_version_file ALL DEPENDS ${STEIKEMANN_VERSION_FILE})
endmacro(generate_version_h)
