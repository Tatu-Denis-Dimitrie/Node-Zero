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
# Fetch Google Test from GitHub
# =============================================================================
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# =============================================================================
# NodeZero.Core Library
# =============================================================================
# Automatically find all source and header files in NodeZero.Core
file(GLOB_RECURSE GAMEAPI_SOURCES
    "NodeZero.Core/*.cpp"
)
file(GLOB_RECURSE GAMEAPI_HEADERS
    "NodeZero.Core/*.h"
)

add_library(NodeZero.Core STATIC
    ${GAMEAPI_SOURCES}
    ${GAMEAPI_HEADERS}
)

target_include_directories(NodeZero.Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeZero.Core
)

# =============================================================================
# NodeZero.UI Executable
# =============================================================================
# Automatically find all source and header files in NodeZero.UI
file(GLOB_RECURSE GAMEUI_SOURCES
    "NodeZero.UI/*.cpp"
)
file(GLOB_RECURSE GAMEUI_HEADERS
    "NodeZero.UI/*.h"
)

add_executable(NodeZero.UI
    ${GAMEUI_SOURCES}
    ${GAMEUI_HEADERS}
)

target_include_directories(NodeZero.UI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeZero.UI
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeZero.Core
)

target_link_libraries(NodeZero.UI PRIVATE
    NodeZero.Core
    raylib
)

# =============================================================================
# Platform-specific settings
# =============================================================================
if(WIN32)
    # Windows-specific settings
    target_link_libraries(NodeZero.UI PRIVATE winmm)
endif()

if(APPLE)
    # macOS-specific settings
    target_link_libraries(NodeZero.UI PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

# =============================================================================
# NodeZero.Tests Executable
# =============================================================================
# Automatically find all test files in NodeZero.Tests
file(GLOB_RECURSE GAMETESTS_SOURCES
    "NodeZero.Tests/*.cpp"
)

add_executable(NodeZero.Tests
    ${GAMETESTS_SOURCES}
)

target_include_directories(NodeZero.Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeZero.Core
)

target_link_libraries(NodeZero.Tests PRIVATE
    NodeZero.Core
    gtest_main
    gtest
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(NodeZero.Tests)

# =============================================================================
# Installation
# =============================================================================
install(TARGETS NodeZero.UI
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
message(STATUS "  Google Test version: 1.14.0")
message(STATUS "  Testing enabled: YES")
message(STATUS "")
