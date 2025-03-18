
set(NAME "steikemann2d")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(RELTYPE "debug")
else()
    set(RELTYPE "release")
endif()

if(WIN32)
    set(OS_TYPE "win32")
elseif(LINUX)
    set(OS_TYPE "linux")
elseif(APPLE)
    set(OS_TYPE "macos")
else()
    message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")

    # TODO support Intel compiler
endif()

set(LIBNAME "${NAME}_${OS_TYPE}_${RELTYPE}")