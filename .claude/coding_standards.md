# Coding Standards

This document defines the coding standards and best practices for the Steikemann2D project.

## C++ Language Standards

- Use C++23 features and standards
- Follow modern C++ best practices
- Leverage godots built in utility features for reference counted resources, nodes, packed arrays and variants.

## Naming Conventions

### General Rules
- Use descriptive, meaningful names
- Avoid abbreviations unless widely understood
- Use PascalCase for types and snake_case for variables/functions

### Specific Rules
- Classes/Structs: `PascalCase`
  ```cpp
  class GameObject;
  struct RenderComponent;
  ```
- Functions/Methods: `snake_case`
  ```cpp
  void update_position();
  bool is_visible() const;
  ```
- Variables: `snake_case`
  ```cpp
  int player_health;
  float move_speed;
  ```
- Constants/Enums: `SCREAMING_SNAKE_CASE`
  ```cpp
  const int MAX_PLAYERS = 4;
  enum class GameState { MENU, PLAYING, PAUSED };
  ```
- Member Variables: `m_` prefix with snake_case
  ```cpp
  class Player {
      int m_health;
      Vector2D m_position;
  };
  ```

## Code Organization

### File Structure
- One class per header file (with rare exceptions)
- Implementation in corresponding .h file
- Group related files in meaningful directories

### Include Order
- Project headers
- C++ standard library
- Third-party libraries
- Local/implementation headers
break between each section

Example:
```h
#include "log.h"
#include "macros.h"
#include "Player.h"

#include <vector>
#include <string>

#include <godot_cpp/classes/Node.hpp>
```

## Code Style

### Formatting
- Use `.clang-format` for consistent formatting
- Max line length: 120 characters
- Indent: 4 spaces (no tabs)
- One statement per line
- Opening brace on same line for functions and control structures

### Comments and Documentation
- Use Doxygen-style comments for public APIs
- Document non-obvious implementations
- Keep comments up-to-date with code changes

Example:
```cpp
/**
 * @brief Updates the entity's position based on velocity and time
 * @param delta_time Time elapsed since last update in seconds
 */
void update_position(float delta_time);
```

## Best Practices

### Memory Management
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Follow RAII principles
- Avoid raw `new` and `delete`
- Use containers from STL

### Error Handling
- Use exceptions for exceptional cases
- Return values for expected failure cases
- Document error conditions and handling

### Performance
- Pass large objects by const reference
- Use move semantics when transferring ownership
- Consider cache coherency in data structures
- Profile before optimizing

### Testing
- Write unit tests for new functionality
- Test edge cases and error conditions
- Keep tests maintainable and readable

## Game-Specific Guidelines

### Game Loop
- Separate update and render logic
- Use fixed timestep for physics
- Handle variable frame rates appropriately

### Component Design
- Follow component-based architecture
- Keep components focused and single-purpose
- Use composition over inheritance

### Resource Management
- Implement proper resource loading/unloading
- Use resource caching when appropriate
- Handle cleanup in destructor or _exit_tree pending on it being an object or node respectively

## Version Control

### Commits
- Write clear, descriptive commit messages
- Keep commits focused and atomic
- Reference issue numbers when applicable

### Branches
- Use feature branches for new development
- Keep main/master branch stable
- Clean up branches after merging

## Review Process

### Code Review Guidelines
- Check for adherence to these standards
- Verify functionality and performance
- Look for potential security issues
- Ensure proper error handling
- Validate documentation

## Safety and Security

### General Guidelines
- Validate all input
- Check array bounds
- Initialize variables
- Use proper access modifiers
- Handle resources safely

### Thread Safety
- Document thread safety requirements
- Use appropriate synchronization
- Avoid global state
- Be explicit about ownership

Remember: These standards are guidelines to help maintain code quality and consistency. Use judgment when applying them, and prioritize readability and maintainability.
