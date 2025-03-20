/* Read by CMake, only change MAJOR/MINOR/PATCH numbers here */
#pragma once
#define STEIKEMANN_MAJOR 0
#define STEIKEMANN_MINOR 0
#define STEIKEMANN_PATCH 3

#define _STEIKEMANN_VERSION_STR_TEXT(text) #text
#define _STEIKEMANN_VERSION_STR(major, minor, patch)                                                                   \
	_STEIKEMANN_VERSION_STR_TEXT(major) "." _STEIKEMANN_VERSION_STR_TEXT(minor) "." _STEIKEMANN_VERSION_STR_TEXT(patch)
#define STEIKEMANN_VERSION_STR _STEIKEMANN_VERSION_STR(STEIKEMANN_MAJOR, STEIKEMANN_MINOR, STEIKEMANN_PATCH)
