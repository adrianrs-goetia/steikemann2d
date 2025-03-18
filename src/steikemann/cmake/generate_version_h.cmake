macro(generate_version_h)
    set(STEIKEMANN_VERSION_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/version.txt")
    set(STEIKEMANN_VERSION_OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h")

    file(READ "${STEIKEMANN_VERSION_INPUT_FILE}" contents)
    string(REPLACE "\n" ";" contents "${contents}")

    foreach(line IN LISTS contents)
        string(REGEX MATCH "([A-Z]+)[ \t]+([0-9]+)" _ "${line}")
        set(key "${CMAKE_MATCH_1}")
        set(value "${CMAKE_MATCH_2}")
        set(VERSION_${key} ${value})
    endforeach()

    set(STEIKEMANN_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

    file(WRITE "${STEIKEMANN_VERSION_OUTPUT_FILE}" "#pragma once\n")
    file(APPEND "${STEIKEMANN_VERSION_OUTPUT_FILE}" "#define STEIKEMANN_MAJOR ${VERSION_MAJOR}\n")
    file(APPEND "${STEIKEMANN_VERSION_OUTPUT_FILE}" "#define STEIKEMANN_MINOR ${VERSION_MINOR}\n")
    file(APPEND "${STEIKEMANN_VERSION_OUTPUT_FILE}" "#define STEIKEMANN_PATCH ${VERSION_PATCH}\n")
    file(APPEND "${STEIKEMANN_VERSION_OUTPUT_FILE}" "#define STEIKEMANN_VERSION ((STEIKEMANN_MAJOR << (8 * 3)) + (STEIKEMANN_MINOR << (8 * 2)) + (STEIKEMANN_PATCH << (8 * 1)))\n")

    message(STATUS "Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

    
endmacro(generate_version_h)