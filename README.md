# NodeZero

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Raylib](https://img.shields.io/badge/Raylib-5.5-red.svg)
![CMake](https://img.shields.io/badge/CMake-3.15%2B-green.svg)

NodeZero is a 2D arcade game built in C++17 with Raylib where players destroy nodes, collect pickups, and push through increasingly challenging levels, inspired by [Nodebuster](https://store.steampowered.com/app/268070/Nodebuster/).

## Screenshots

<div align="center">
  <img src="docs/images/gameplay1.png" alt="Gameplay Screenshot 1" width="100%">
  <img src="docs/images/gameplay2.png" alt="Gameplay Screenshot 2" width="100%">
</div>

---

## Highlights

- Fast-paced gameplay built around a mouse-controlled damage zone
- Multiple enemy shapes and level bosses
- Persistent upgrades and coin-based progression
- Automatic save system
- Clean architecture with strict separation between core and UI
- Comprehensive test suite for core logic

## How to Play

**Controls**

- **Mouse Movement** – moves the damage zone
- **ESC** – pause / settings menu

**Objective**

Survive as long as possible by destroying enemy nodes before they escape the screen. Progress through levels, manage health, and upgrade your abilities.

**Core Loop**

1. **Destroy Nodes** – keep the damage zone over enemies
2. **Collect Pickups** – earn points and coins
3. **Clear Levels** – defeat the boss to advance

**Upgrades**

- **Health Upgrade (50 coins)** – increases max health
- **Regeneration Upgrade (100 coins)** – restores health over time
- **Damage Upgrade (60 coins)** – increases damage per tick
- **Damage Zone Upgrade (75 coins)** – expands the damage zone

**Save Data**

- **Windows:** `%APPDATA%\NodeZero\save.dat`
- **Linux/macOS:** `~/.config/NodeZero/save.dat`

## Quick Play (Release Build)

1. Open the [Releases](../../releases) page on GitHub
2. Download the latest Windows build
3. Extract the archive
4. Run the executable

## Build & Run

**Prerequisites:** CMake 3.15+, Visual Studio 2022 (Windows) or GCC 8+/Clang 7+ (Linux/macOS)

```bash
# Build
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug

# Run (Windows)
build\bin\Debug\NodeZero.UI.exe
# Or with forward slashes (Git Bash)
./build/bin/Debug/NodeZero.UI.exe

# (Tests are covered in the Testing section below)
```

**VS Code:** `Ctrl+Shift+B` to build, `F5` to debug (launch configs in `.vscode/launch.json`)

**Visual Studio:** Open folder → `Ctrl+Shift+B` → `F5`

---

## Architecture

Three-layer separation:

```
NodeZero.Core/    → platform-agnostic game logic
NodeZero.UI/      → rendering + input
NodeZero.Tests/   → Google Test suite
```

The core exposes interfaces (`IGame`, `INode`) consumed by the UI. The event system uses the Observer pattern for decoupled communication.

## Project Structure

```
NodeZero.Core/
├── include/
│   ├── Config/GameConfig.h          # Balance constants
│   ├── Enums/                       # NodeShape, NodeState, GameScreen, EventType
│   ├── Events/                      # Observer pattern
│   ├── Services/                    # Service interfaces
│   ├── Types/                       # Data structures
│   └── IGame.h, INode.h             # Core interfaces
└── src/
    ├── Game.cpp, Node.cpp
    ├── Events/Subject.cpp
    └── Services/

NodeZero.UI/
├── include/
│   ├── Screens/                     # Screen states (Gameplay, Menu, Pause)
│   ├── Widgets/                     # UI components
│   └── GameApp.h, Renderer.h, InputHandler.h
└── src/ + main.cpp

NodeZero.Tests/
├── EnemyTests.cpp
├── ServiceTests.cpp
├── LevelAndSpawnTests.cpp
├── PickupAndDamageTests.cpp
└── GameTests.cpp
```

**Dependencies:** CMake automatically fetches Raylib 5.5 and Google Test 1.14.0
**Testing:** 83 tests covering core logic and integration scenarios

## Configuration

Balance tuning lives in [NodeZero.Core/include/Config/GameConfig.h](NodeZero.Core/include/Config/GameConfig.h):

```cpp
static constexpr float NODE_DEFAULT_SPEED = 75.0f;
static constexpr int POINTS_MULTIPLIER_MAX = 5;
static constexpr int HEALTH_UPGRADE_COST = 50;
static constexpr float DAMAGE_ZONE_UPGRADE_AMOUNT = 10.0f;
```

## Testing

```bash
# Run all tests (Windows)
build\bin\Debug\NodeZero.Tests.exe
# Or with forward slashes (Git Bash)
./build/bin/Debug/NodeZero.Tests.exe

# Filter tests
build\bin\Debug\NodeZero.Tests.exe --gtest_filter=GameTest.*

# List tests
build\bin\Debug\NodeZero.Tests.exe --gtest_list_tests

# CTest
ctest --test-dir build -C Debug --output-on-failure
```

---

## Troubleshooting

**Build fails?**
Clear cache: `rm -rf build && cmake -B build -G "Visual Studio 17 2022"`

**Game crashes on start?**
Check assets in `build/bin/Debug/assets/`

**Tests not visible in Visual Studio?**
Rebuild: `cmake --build build --config Debug --target NodeZero.Tests`

**Reset progress?**
Delete the save file:

- **Windows:** `%APPDATA%\NodeZero\save.dat`
- **Linux/macOS:** `~/.config/NodeZero/save.dat`

---

## Credits

NodeZero was developed collaboratively by a student team in an academic setting.  
The project includes shared contributions across gameplay systems, core logic, and user interface.


## Contributing

Contributions, suggestions, and feedback are welcome. Feel free to open an issue or a pull request.
