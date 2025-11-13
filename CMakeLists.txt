cmake_minimum_required(VERSION 3.15)
project(NodeZero VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =============================================================================
# Fetch Raylib from GitHub
# =============================================================================
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)

# Disable building Raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# =============================================================================
# GameAPI Library
# =============================================================================
add_library(GameAPI STATIC
    # Core
    GameAPI/Core/GameFactory.cpp
    GameAPI/Core/GameFactory.h
    GameAPI/Core/IGame.h
    GameAPI/Core/IPlayer.h

    # Entities
    GameAPI/Entities/Game.cpp
    GameAPI/Entities/Game.h
    GameAPI/Entities/Player.cpp
    GameAPI/Entities/Player.h

    # Main header
    GameAPI/GameAPI.h
)

target_include_directories(GameAPI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/GameAPI
)

# =============================================================================
# GameUI Executable
# =============================================================================
add_executable(GameUI
    GameUI/main.cpp
    GameUI/Rendering/PlayerRenderer.cpp
    GameUI/Rendering/PlayerRenderer.h
    GameUI/UI/HUD.cpp
    GameUI/UI/HUD.h
)

target_include_directories(GameUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/GameUI
    ${CMAKE_CURRENT_SOURCE_DIR}/GameAPI
)

target_link_libraries(GameUI PRIVATE
    GameAPI
    raylib
)

# =============================================================================
# Platform-specific settings
# =============================================================================
if(WIN32)
    # Windows-specific settings
    target_link_libraries(GameUI PRIVATE winmm)
endif()

if(APPLE)
    # macOS-specific settings
    target_link_libraries(GameUI PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

# =============================================================================
# Installation
# =============================================================================
install(TARGETS GameUI
    RUNTIME DESTINATION bin
)

# Copy assets if they exist
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    install(DIRECTORY assets DESTINATION bin)
endif()

# =============================================================================
# Print configuration summary
# =============================================================================
message(STATUS "")
message(STATUS "NodeZero Configuration:")
message(STATUS "  CMake version: ${CMAKE_VERSION}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Raylib version: 5.5")
message(STATUS "")
