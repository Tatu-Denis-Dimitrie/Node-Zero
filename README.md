# NodeZero

A 2D arcade game built with C++17 and Raylib 5.5. Destroy nodes, collect pickups, and progress through levels.

Inspired by [Nodebuster](https://store.steampowered.com/app/268070/Nodebuster/) on Steam.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Raylib](https://img.shields.io/badge/Raylib-5.5-red.svg)
![CMake](https://img.shields.io/badge/CMake-3.15%2B-green.svg)

## Quick Start

**Prerequisites:** CMake 3.15+, Visual Studio 2022 (Windows) or GCC 8+/Clang 7+ (Linux/macOS)

```bash
# Build
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug

# Run
./build/bin/Debug/NodeZero.UI.exe

# Test
./build/bin/Debug/NodeZero.Tests.exe
```

**VS Code:** `Ctrl+Shift+B` to build, `F5` to debug (launch configs in `.vscode/launch.json`)

**Visual Studio:** Open folder → `Ctrl+Shift+B` → `F5`

## Gameplay

Destroy nodes by hovering over them with your damage zone (mouse cursor). Nodes spawn from the right and move left - if they escape, you lose health. Destroyed nodes drop pickups for coins and points.

**Progression:** Complete levels by destroying the required number of nodes. Spend coins on upgrades (health, regeneration, damage, zone size) to handle increasing difficulty.

## Architecture

Three-layer clean architecture with strict separation:

```
NodeZero.Core/    → Pure game logic (no rendering, platform-agnostic)
NodeZero.UI/      → Raylib rendering + input handling
NodeZero.Tests/   → Google Test suite
```

Core exposes interfaces (`IGame`, `INode`, `ILevel`) consumed by UI. Event system uses Observer pattern for decoupled communication.

## Project Structure

```
NodeZero.Core/
├── include/
│   ├── Config/GameConfig.h          # Tuning constants
│   ├── Enums/                       # NodeShape, NodeState, GameScreen
│   ├── Events/                      # Observer pattern implementation
│   ├── Systems/                     # SaveSystem, CollisionSystem, ScoreSystem
│   ├── Types/                       # Data structures (Position, SaveData, PointPickup)
│   └── IGame.h, INode.h, ILevel.h   # Core interfaces
└── src/
    └── Game.cpp, Node.cpp, Level.cpp, Systems/

NodeZero.UI/
├── include/Screens/                 # Screen states (Gameplay, MainMenu, Pause, etc.)
├── include/Widgets/                 # UI components (Button, Label, Menu)
└── src/ + main.cpp

NodeZero.Tests/
└── GameTests.cpp, PositionTests.cpp
```

**Dependencies:** CMake auto-fetches Raylib 5.5 and Google Test 1.14.0

## Development

### Rules

- **Core isolation:** Never include Raylib or rendering code in `NodeZero.Core`
- **Interface-driven:** UI depends on abstractions (`IGame`), not implementations (`Game`)
- **Stateless systems:** Pass data as parameters, no global state
- **Test coverage:** Add tests for new core logic

### Code Conventions

```cpp
// Classes & Interfaces
class GameApp { ... };
class IGame { ... };

// Members & Functions
float m_CurrentHealth;
void SpawnNode(float x, float y);

// Constants
static constexpr float NODE_SPAWN_INTERVAL = 2.0f;
```

### Configuration

Game balance tuning in [GameConfig.h](NodeZero.Core/include/Config/GameConfig.h):

```cpp
static constexpr float NODE_SPAWN_INTERVAL = 2.0f;
static constexpr int POINTS_PER_NODE_DESTROYED = 100;
static constexpr int HEALTH_UPGRADE_COST = 50;
static constexpr float DAMAGE_ZONE_UPGRADE_AMOUNT = 10.0f;
```

## Testing

```bash
# Run all tests
./build/bin/Debug/NodeZero.Tests.exe

# Filter tests
./build/bin/Debug/NodeZero.Tests.exe --gtest_filter=GameTest.*

# CTest integration
ctest --test-dir build -C Debug --output-on-failure
```

## Troubleshooting

**Build fails?**
Clear cache: `rm -rf build && cmake -B build -G "Visual Studio 17 2022"`

**Game crashes on start?**
Check assets copied: `build/bin/Debug/assets/`
Verify Raylib: `build/_deps/raylib-src/`

**Tests not found in VS?**
Rebuild: `cmake --build build --config Debug --target NodeZero.Tests`

---

Built with [Raylib](https://www.raylib.com/)
