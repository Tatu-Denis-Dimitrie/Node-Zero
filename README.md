# NodeZero

A 2D game built with Raylib and C++.

## Quick Start

### Prerequisites

-   Visual Studio 2022 with C++ Desktop Development
-   CMake 3.15+

### Setup & Run

```bash
# Generate build files and download Raylib
cmake -B build -S .

# Build Raylib (first time only)
cmake --build build --config Debug
cmake --build build --config Release

# Build and run in VS Code
# Ctrl+Shift+B to build
# F5 to run
```

## Project Structure

```
GameAPI/    # Game logic (Core + Entities)
GameUI/     # UI + Rendering + entry point
build/      # Generated files (like node_modules)
```
