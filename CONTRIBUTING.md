# Contributing to Fallout 2 Community Edition

Thank you for your interest in contributing to Fallout 2 Community Edition! This guide will help you set up your development environment and understand the codebase.

## Table of Contents

- [Getting Started](#getting-started)
- [Building the Project](#building-the-project)
  - [Windows](#windows)
  - [Linux](#linux)
  - [macOS](#macos)
  - [iOS](#ios)
  - [Android](#android)
- [Code Style and Formatting](#code-style-and-formatting)
- [Architecture Overview](#architecture-overview)
- [Naming Conventions](#naming-conventions)
- [Development Workflow](#development-workflow)
- [Contribution Guidelines](#contribution-guidelines)

## Getting Started

### Prerequisites

Before you start, ensure you have the following tools installed:

- **Git**: For version control
- **CMake**: Version 3.13 or higher (3.19+ recommended for presets)
- **C++17 compatible compiler**:
  - Windows: Visual Studio 2019 or later
  - Linux: GCC or Clang with C++17 support
  - macOS: Xcode 10.13 or later
- **SDL2**: Required for graphics and input (version 2.0 or higher)
- **zlib**: For compression support

### Clone the Repository

```bash
git clone https://github.com/alexbatalov/fallout2-ce.git
cd fallout2-ce
```

## Building the Project

The project uses CMake as its build system with platform-specific presets defined in `CMakePresets.json`.

### Windows

#### Prerequisites
- Visual Studio 2019 or later (Community Edition is fine)
- CMake 3.19 or higher

#### Build Steps

1. **Open a Command Prompt or PowerShell** in the project root directory.

2. **Configure the build** (choose x86 or x64):
   ```cmd
   # For 32-bit (x86)
   cmake --preset windows-x86
   
   # For 64-bit (x64)
   cmake --preset windows-x64
   ```

3. **Build the project**:
   ```cmd
   # Debug build
   cmake --build --preset windows-x64-debug
   
   # Release build
   cmake --build --preset windows-x64-release
   ```

4. **Find the executable** in:
   - Debug: `out/build/windows-x64/Debug/fallout2-ce.exe`
   - Release: `out/build/windows-x64/Release/fallout2-ce.exe`

5. **Copy the executable** to your Fallout 2 installation directory to run it.

### Linux

#### Prerequisites
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake libsdl2-dev zlib1g-dev

# For 32-bit builds on 64-bit systems
sudo dpkg --add-architecture i386
sudo apt install g++-multilib libsdl2-dev:i386 zlib1g-dev:i386

# Fedora/RHEL
sudo dnf install gcc-c++ cmake SDL2-devel zlib-devel
```

#### Build Steps

1. **Configure the build**:
   ```bash
   # 64-bit Debug
   cmake --preset linux-x64-debug
   
   # 64-bit Release
   cmake --preset linux-x64-release
   
   # 32-bit Debug
   cmake --preset linux-x86-debug
   
   # 32-bit Release
   cmake --preset linux-x86-release
   ```

2. **Build the project**:
   ```bash
   cmake --build --preset linux-x64-debug
   ```

3. **Find the executable** in `out/build/linux-x64-debug/fallout2-ce`

4. **Run the game**:
   ```bash
   # Copy the executable to your Fallout 2 directory
   cp out/build/linux-x64-debug/fallout2-ce /path/to/Fallout2/
   cd /path/to/Fallout2/
   ./fallout2-ce
   ```

#### ARM Linux (Raspberry Pi, etc.)

For ARM builds, you'll need cross-compilation tools or build natively on ARM hardware:

```bash
# ARM64 (native build on ARM64 system)
cmake -B build -D CMAKE_BUILD_TYPE=Debug -D FALLOUT_VENDORED=OFF
cmake --build build

# ARMhf (cross-compile)
cmake -B build \
  -D CMAKE_BUILD_TYPE=Debug \
  -D FALLOUT_VENDORED=OFF \
  -D CMAKE_SYSTEM_PROCESSOR=arm \
  -D CMAKE_C_COMPILER=arm-linux-gnueabihf-gcc \
  -D CMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++
cmake --build build
```

### macOS

#### Prerequisites
- Xcode 10.13 or later (includes Clang compiler)
- CMake 3.19 or higher

You can install CMake via Homebrew:
```bash
brew install cmake
```

#### Build Steps

1. **Configure the build**:
   ```bash
   cmake --preset macos
   ```

2. **Build the project**:
   ```bash
   # Debug build
   cmake --build --preset macos-debug
   
   # Release build
   cmake --build --preset macos-release
   ```

3. **Package as DMG** (optional):
   ```bash
   cd out/build/macos
   cpack -C Debug
   ```

4. **Find the app bundle** in `out/build/macos/Debug/Fallout II Community Edition.app`

5. **Run or install**:
   - Double-click the `.app` to run directly
   - Or copy it to your Fallout 2 game directory

**Note**: The macOS build creates a universal binary supporting both Intel (x86_64) and Apple Silicon (arm64) architectures.

### iOS

#### Prerequisites
- macOS with Xcode 12 or later
- iOS deployment target: 12.0 or higher
- CMake 3.19 or higher

#### Build Steps

1. **Configure for iOS**:
   ```bash
   cmake --preset ios
   ```

2. **Build the project**:
   ```bash
   # Debug build
   cmake --build --preset ios-debug
   
   # Release build
   cmake --build --preset ios-release
   ```

3. **Package as IPA**:
   ```bash
   cd out/build/ios
   cpack -C Debug
   ```

4. **Install on device**:
   - Use Xcode to open the project and deploy to a device
   - Or use sideloading tools like [AltStore](https://altstore.io/) or [Sideloadly](https://sideloadly.io/)
   - The IPA file will be at `out/build/ios/fallout2-ce.ipa`

**Note**: You'll need to copy game data files (master.dat, critter.dat, etc.) to the app via iTunes File Sharing or Finder after installation.

### Android

#### Prerequisites
- Java Development Kit (JDK) 11
- Android SDK (automatically downloaded by Gradle if not present)
- Gradle (wrapper included in the project)

#### Build Steps

1. **Navigate to Android directory**:
   ```bash
   cd os/android
   ```

2. **Build APK**:
   ```bash
   # Debug build
   ./gradlew assembleDebug
   
   # Release build (requires signing configuration)
   ./gradlew assembleRelease
   ```

3. **Find the APK** in `os/android/app/build/outputs/apk/debug/app-debug.apk`

4. **Install on device**:
   ```bash
   # Using adb
   adb install app/build/outputs/apk/debug/app-debug.apk
   ```

**Note**: On first run, the app will prompt you to select the Fallout 2 game data directory on your device.

## Code Style and Formatting

The project follows these coding standards:

### Formatting Rules

- **Indentation**: 4 spaces (no tabs)
- **Line endings**: LF (Unix-style)
- **Encoding**: UTF-8
- **Final newline**: Required in all files
- **Trailing whitespace**: Remove all trailing whitespace

These rules are defined in `.editorconfig` and enforced by most modern editors.

### C++ Style

The project uses a **WebKit-based** coding style with some modifications. See `.clang-format` for details.

**Format your code before committing**:
```bash
# Check formatting
find src -type f \( -name "*.cc" -o -name "*.h" \) | xargs clang-format --dry-run --Werror

# Auto-format files
find src -type f \( -name "*.cc" -o -name "*.h" \) | xargs clang-format -i
```

### Static Analysis

Run cppcheck before committing:
```bash
cppcheck --std=c++17 src/
```

## Architecture Overview

Fallout 2 CE is a re-implementation of the original Fallout 2 engine, reverse-engineered from the original binary. This results in some unique architectural patterns.

### Project Structure

```
fallout2-ce/
├── src/                    # Source code
│   ├── main.cc            # Entry point and main game loop
│   ├── game.cc/h          # Core game module
│   ├── object.cc/h        # Game object system
│   ├── scripts.cc/h       # Script system integration
│   ├── sfall_*.cc/h       # Sfall compatibility layer
│   └── ...                # Other game systems
├── os/                     # Platform-specific files
│   ├── windows/           # Windows resources
│   ├── macos/             # macOS bundle resources
│   ├── ios/               # iOS resources
│   └── android/           # Android project
├── third_party/           # Vendored dependencies
└── CMakeLists.txt         # Build configuration
```

### Key Components

#### 1. Entry Point and Game Loop

**File**: `src/main.cc`

The game initialization follows this sequence:
1. `falloutMain()` - Main entry point
2. `falloutInit()` - Initialize game systems
3. Movie playback (if enabled)
4. Main menu (`mainMenuWindowInit()`)
5. Game loop (`mainLoop()`)
6. Cleanup (`main_exit_system()`)

The **main game loop** (`mainLoop()`) runs at approximately 60 FPS and processes:
- Input events (`inputGetInput()`)
- Script hooks (`sfall_gl_scr_process_main()`)
- Game state updates (`gameHandleKey()`)
- Script requests (`scriptsHandleRequests()`)
- Map transitions (`mapHandleTransition()`)
- Rendering (`renderPresent()`)

#### 2. Module Organization

**Pattern**: The codebase uses a **C-style module pattern** rather than traditional OOP classes:
- Each module has a `.h` header and `.cc` implementation file
- Global state is managed via static variables within modules
- Functions are the primary abstraction (not classes)

**Example** from `game.cc`:
```cpp
// Global game state (prefixed with 'g')
static Object* gDude = nullptr;
static int gGameGlobalVars[GVAR_COUNT];

// Module initialization
int gameInit() {
    // Initialize game systems
    return 0;  // 0 = success, -1 = error
}
```

#### 3. Object System

**Files**: `src/object.h`, `src/object.cc`

Objects represent all entities in the game world:
- **Items** (weapons, armor, consumables)
- **Critters** (NPCs, enemies, the player character "Dude")
- **Scenery** (doors, containers, decorations)
- **Tiles** (floor and wall textures)

**Key patterns**:
- Opaque pointer type: `Object*`
- Iterator pattern: `objectFindFirst()` → `objectFindNext()`
- Public API uses `object*()` prefix
- Internal functions use `_obj*()` prefix

#### 4. Script System

**Files**: `src/scripts.h`, `src/scripts.cc`

Fallout uses a compiled scripting system with these script types:
- **SYSTEM**: Global game events
- **SPATIAL**: Location-triggered events
- **TIMED**: Time-based events
- **ITEM**: Item interactions
- **CRITTER**: NPC behavior and dialogue

Scripts execute **procedures** (numbered entry points) in response to game events:
- `SCRIPT_PROC_START` - Script initialization
- `SCRIPT_PROC_TALK` - Dialogue interaction
- `SCRIPT_PROC_USE` - Item/object usage
- And many more...

#### 5. Game Modes

**Pattern**: RAII-based mode management

The game can be in multiple modes simultaneously (bitflags):
- `kWorldmap` - Worldmap view
- `kDialog` - In conversation
- `kCombat` - Combat mode
- `kInventory` - Inventory screen
- `kPipboy` - Pip-Boy interface

Use `ScopedGameMode` for automatic mode cleanup.

#### 6. Sfall Integration

**Files**: `src/sfall_*.cc`

Sfall is a popular Fallout 2 mod that adds scripting features and bug fixes. This project integrates Sfall features for compatibility with mods:
- **Opcodes**: Extended scripting commands (`sfall_opcodes.cc`)
- **Global scripts**: Persistent script execution (`sfall_global_scripts.cc`)
- **Arrays**: Dynamic array support for scripts (`sfall_arrays.cc`)
- **Metarules**: Game engine hooks (`sfall_metarules.cc`)

#### 7. Reverse Engineering Heritage

Many functions include address comments like `// 0x501C9C` - these reference offsets in the original Fallout 2 binary and help track the source of reverse-engineered code. **These should be preserved** when modifying existing functions.

## Naming Conventions

Understanding the naming conventions is crucial for contributing to this codebase:

| Pattern | Example | Usage |
|---------|---------|-------|
| `gVariableName` | `gDude`, `gGameGlobalVars` | **Global variables** across the entire program |
| `_variableName` | `_game_user_wants_to_quit` | **Static module-level variables** (file scope) |
| `camelCase()` | `inputGetInput()`, `mapInit()` | **Public functions** callable from other modules |
| `_snake_case()` | `_obj_move()`, `_game_handle_input()` | **Private/internal functions** within a module |
| `kConstantName` | `kWorldmap`, `kDialog` | **Enum constants** (modern C++ style) |
| `UPPER_CASE` | `GAME_TIME_TICKS_PER_HOUR` | **Macros and legacy constants** |
| `// 0xABCDEF` | `// 0x501C9C` | **Original binary address** (reverse engineering marker) |

### Common Prefixes

Functions are often prefixed by their module name:
- `object*()` - Object system operations
- `map*()` - Map/level operations
- `script*()` - Script system
- `game*()` - Core game functions
- `art*()` - Graphics/artwork management
- `animation*()` - Animation system

### Fallout-Specific Terms

- **Dude**: The player character (global: `gDude`)
- **Critter**: Any animated creature (NPCs, enemies, player)
- **Elevation**: Maps have 3 elevation levels (0, 1, 2)
- **Proto**: Prototype/template for creating objects
- **FID**: Fallout ID - unique identifier for game resources
- **Tile**: Hex-based map tile
- **SPECIAL**: Character stats (Strength, Perception, Endurance, Charisma, Intelligence, Agility, Luck)

## Development Workflow

### Setting Up Your Environment

1. **Fork the repository** on GitHub
2. **Clone your fork**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/fallout2-ce.git
   cd fallout2-ce
   ```
3. **Add upstream remote**:
   ```bash
   git remote add upstream https://github.com/alexbatalov/fallout2-ce.git
   ```
4. **Build the project** following the platform-specific instructions above

### Making Changes

1. **Create a feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes** following the code style guidelines

3. **Test your changes**:
   - Build the project
   - Run the game and verify your changes work
   - Test on multiple platforms if possible

4. **Format your code**:
   ```bash
   find src -type f \( -name "*.cc" -o -name "*.h" \) | xargs clang-format -i
   ```

5. **Run static analysis**:
   ```bash
   cppcheck --std=c++17 src/
   ```

6. **Commit your changes**:
   ```bash
   git add .
   git commit -m "Brief description of your changes"
   ```

### Submitting Changes

1. **Push your branch** to your fork:
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Open a Pull Request** on GitHub:
   - Go to the original repository
   - Click "New Pull Request"
   - Select your fork and branch
   - Fill in the PR template with details about your changes

3. **Respond to feedback**:
   - Address code review comments
   - Update your PR by pushing new commits to your branch

## Contribution Guidelines

### What We're Looking For

**High Priority**:
- **Sfall feature integration**: Compatibility with popular mods
- **Bug fixes**: Gameplay bugs, crashes, or incorrect behavior
- **Quality of life improvements**: UI enhancements, better controls, etc.
- **Platform-specific fixes**: Issues on specific operating systems

**Please Avoid**:
- **Large-scale refactoring**: We need to stay compatible with upstream changes
- **Style-only changes**: Unless fixing specific style violations
- **Breaking changes**: Must maintain save game compatibility

### Before Starting Work

**Open an issue** to discuss your proposed changes, especially for:
- New features
- Significant bug fixes
- Architectural changes

This prevents duplicate work and ensures your contribution aligns with project goals.

### Code Review Process

All contributions go through code review:
1. **Automated checks** run via GitHub Actions (formatting, static analysis, builds)
2. **Manual review** by maintainers
3. **Testing** on multiple platforms (if applicable)
4. **Merge** once approved

### Testing

While there's no automated test suite, please test your changes thoroughly:
- **Compile** on your platform
- **Run the game** and verify functionality
- **Test edge cases** for your changes
- **Check for regressions** in related areas

If possible, test on multiple platforms (Windows, Linux, macOS).

### Documentation

Update documentation when you:
- Add new configuration options
- Change build requirements
- Modify game behavior
- Add new features

### Commit Messages

Write clear commit messages:
```
Brief summary (50 chars or less)

More detailed explanation if needed. Wrap at 72 characters.
Explain what changed and why, not how.

- Bullet points are okay
- Reference issues: Fixes #123
```

### Code Comments

- **Preserve** existing address comments (`// 0xABCDEF`)
- **Add comments** for non-obvious code
- **Explain why**, not what (the code shows what)
- **Document** complex algorithms or game mechanics

### License

By contributing, you agree that your contributions will be licensed under the [Sustainable Use License](LICENSE.md).

---

## Getting Help

- **Issues**: Check existing issues or open a new one
- **Discussions**: Use GitHub Discussions for questions
- **Community**: Join community forums and Discord (links in README)

Thank you for contributing to Fallout 2 Community Edition! Your efforts help preserve this classic game for future generations.
