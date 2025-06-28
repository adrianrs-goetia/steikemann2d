# Project Overview Helper

This document provides key context about the Steikemann2D project structure and development patterns.

## Project Structure

- `.claude/` - Helper files for Claude assistance
- `bin/` - Binary output directory
- `build/` - CMake build directory
- `cpp/` - Main C++ source code
- `gdd/` - Game design documentation
- `project/` - Project configuration and assets

## Key Components

### Build System
- Uses CMake for build configuration
- Supports both Linux and Windows (MinGW) builds
- Build presets defined in `CMakePresets.json`

### Code Organization
- Modern C++ project using C++23
- Code formatting follows `.clang-format` style
- Language server configuration in `.clangd`

### Development Guidelines

1. Follow existing code style and patterns
2. Keep code modular and well-documented
3. Update relevant documentation when making changes
4. Use meaningful variable and function names
5. Add test coverage for new functionality

### Common Patterns

- Use `const` for immutable values
- Prefer references over pointers when possible.
- Keep all implementation in header files, do not break symbol declaration and definition into .h/.cpp.
- Follow RAII principles
- Use godot::Ref pointers for memory management to objects inheriting from godot::RefCounted
- Use godot::NodePath with godot::Node::get_node(<nodepath>) instead of raw pointers to Nodes
- Do not use exceptions for error handling

### Documentation

- Main game design document: `gdd.md`
- Technical documentation should be in source files
- README.md contains setup and build instructions

## Quick Reference

### Important Files
- `CMakeLists.txt` - Main build configuration
- `.clang-format` - Code style configuration
- `mingw-toolchain.cmake` - Windows build toolchain

### Build Commands
```bash
# Configure build
cmake --preset=gcc-debug

# Build from project root utilizing the Makefile
make build
```

This overview provides essential context for working with the Steikemann2D project. Use it as a reference when providing assistance or making code changes.
