macro(set_libname)
    set(NAME "steikemann2d")

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(BUILD_TYPE "debug")
    else()
        set(BUILD_TYPE "release")
    endif()

    # Write all lib names for gdextension.template
    set(LIBNAME "${NAME}_${BUILD_TYPE}")

    message(STATUS "libname: ${LIBNAME}")
endmacro(set_libname)