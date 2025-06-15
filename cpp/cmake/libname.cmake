macro(set_libname)
    set(NAME "steikemann2d")

    if (NOT MINGW)
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            set(BUILD_TYPE "_debug")
        else()
            set(BUILD_TYPE "_release")
        endif()
    endif()

    # Write all lib names for gdextension.template
    set(LIBNAME "${NAME}${BUILD_TYPE}")

    message(STATUS "libname: ${LIBNAME}")
endmacro(set_libname)
