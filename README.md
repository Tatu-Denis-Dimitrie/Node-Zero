# NodeZero

A 2D game built with Raylib and C++.

## Quick Start

### Prerequisites

-   Visual Studio 2022 with C++ Desktop Development
-   CMake 3.15+

### Setup & Run

```bash
# Generate build files
cmake -B build -G "Visual Studio 17 2022"

# Build and run in VS Code
# Ctrl+Shift+B to build
# F5 to run
```

## Project Structure

```
NodeZero.Core/   # Game logic (Core + Entities)
NodeZero.Tests/  # Tests
NodeZero.UI/     # UI + Rendering + entry point
build/           # Generated files (like node_modules)
```
