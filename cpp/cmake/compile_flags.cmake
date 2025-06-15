function(
    set_steikemann_compile_flags
    project_name
    )
    set(STEIKEMANN_COMPILE_FLAGS)
    target_compile_definitions(${project_name} PRIVATE -DFMT_HEADER_ONLY)

    steikemann_set_project_warnings(${project_name} ON "" "" "" "")

    # Compilation commands copied from godot-cpp
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # message("TODO! Windows build")

        # # using Visual Studio C++
        # set(STEIKEMANN_COMPILE_FLAGS "/utf-8") # /GF /MP

        # if(CMAKE_BUILD_TYPE MATCHES Debug)
        # set(STEIKEMANN_COMPILE_FLAGS "${STEIKEMANN_COMPILE_FLAGS} /MDd") # /Od /RTC1 /Zi
        # else()
        # set(STEIKEMANN_COMPILE_FLAGS "${STEIKEMANN_COMPILE_FLAGS} /MD /O2") # /Oy /GL /Gy
        # endif(CMAKE_BUILD_TYPE MATCHES Debug)

        # add_definitions(-DNOMINMAX)
    else() # GCC/Clang/MINGW
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            set(STEIKEMANN_COMPILE_FLAGS "${STEIKEMANN_COMPILE_FLAGS} -fno-omit-frame-pointer -O0 -g")
        else()
            set(STEIKEMANN_COMPILE_FLAGS "${STEIKEMANN_COMPILE_FLAGS} -Os, -s -flto -fdata-sections -ffunction-sections -Wl, --gc-sections -static")
        endif(CMAKE_BUILD_TYPE MATCHES Debug)
    endif()

    # Disable exception handling
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # message("TODO! Windows build")

        # set(STEIKEMANN_COMPILE_FLAGS "${STEIKEMANN_COMPILE_FLAGS} -D_HAS_EXCEPTIONS=0")
    else()
        set(STEIKEMANN_COMPILE_FLAGS "${STEIKEMANN_COMPILE_FLAGS} -fno-exceptions")
    endif()

    target_compile_options(
        ${project_name}
        INTERFACE # C++ warnings
        $<$<COMPILE_LANGUAGE:CXX>:${STEIKEMANN_COMPILE_FLAGS}>

        # C warnings
        $<$<COMPILE_LANGUAGE:C>:${STEIKEMANN_COMPILE_FLAGS}>
    )

endfunction(set_steikemann_compile_flags)
