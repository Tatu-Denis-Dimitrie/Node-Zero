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
# GameAPI Library
# =============================================================================
# Automatically find all source and header files in GameAPI
file(GLOB_RECURSE GAMEAPI_SOURCES 
    "GameAPI/*.cpp"
)
file(GLOB_RECURSE GAMEAPI_HEADERS 
    "GameAPI/*.h"
)

add_library(GameAPI STATIC
    ${GAMEAPI_SOURCES}
    ${GAMEAPI_HEADERS}
)

target_include_directories(GameAPI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/GameAPI
)

# =============================================================================
# GameUI Executable
# =============================================================================
# Automatically find all source and header files in GameUI
file(GLOB_RECURSE GAMEUI_SOURCES 
    "GameUI/*.cpp"
)
file(GLOB_RECURSE GAMEUI_HEADERS 
    "GameUI/*.h"
)

add_executable(GameUI
    ${GAMEUI_SOURCES}
    ${GAMEUI_HEADERS}
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
# GameTests Executable
# =============================================================================
# Automatically find all test files in GameTests
file(GLOB_RECURSE GAMETESTS_SOURCES
    "GameTests/*.cpp"
)

add_executable(GameTests
    ${GAMETESTS_SOURCES}
)

target_include_directories(GameTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/GameAPI
)

target_link_libraries(GameTests PRIVATE
    GameAPI
    gtest_main
    gtest
)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(GameTests)

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
message(STATUS "  Google Test version: 1.14.0")
message(STATUS "  Testing enabled: YES")
message(STATUS "")
