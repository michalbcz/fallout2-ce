# Tutorial 2: Fallout2-CE Architecture

Understanding how the `fallout2-ce` project is structured is vital for navigating the codebase and making changes. This tutorial covers the high-level architecture, the main game loop, and how various systems interact.

## 1. Directory Structure

The repository is organized functionally. Here are the most important directories:

- **`src/`**: This is the heart of the engine. It contains all the reverse-engineered C++ code that replaces the original Fallout 2 executable.
- **`third_party/`**: Contains embedded dependencies. Notably, this is where the `quickjs` (QuickJS) engine lives, which powers the modern JavaScript scripting support.
- **`os/`**: Contains platform-specific wrappers and abstractions (e.g., handling paths on Windows vs Linux/macOS).
- **`examples/js/`**: Contains examples and documentation for the JavaScript API available to modders.

## 2. Core Dependencies: SDL2

The original Fallout 2 was written for Windows 95/98 using DirectX. To make it cross-platform, `fallout2-ce` entirely replaces the rendering, audio, and input backends with **SDL2 (Simple DirectMedia Layer)**.
- **Video:** SDL2 handles window creation and basic rendering (often upscaled). See files like `src/window.cc` and `src/svga.cc`.
- **Audio:** SDL2_mixer is used to play sound effects and music. See `src/game_sound.cc`.
- **Input:** Keyboard and mouse events are intercepted by SDL and routed into the game's internal input queues. See `src/input.cc`, `src/mouse.cc`, and `src/kb.cc`.

*Note: As mentioned in the first tutorial, integration with SDL requires passing game data through `SDL_RWops` wrappers because assets are stored in the Virtual File System (`.dat` files).*

## 3. The Game Loop

All games run on a continuous loop that processes input, updates the game state, and renders the screen. In `fallout2-ce`, the entry point is standard:

1. **`main.cc`**: The `main()` function initializes the system, loads configuration preferences (`fallout2.cfg`), initializes the VFS (`db.cc`), and sets up SDL.
2. **`game.cc` / `game_loop()`**: The core runtime loop lives here. During the loop, the engine:
   - Processes user input (mouse clicks, keyboard presses).
   - Executes scripts (`src/scripts.cc` and `src/js_integration.cc`). Scripts control NPC AI, dialogue, and map events.
   - Updates entity animations and physics.
   - Renders the current view of the world (`src/map.cc`, `src/draw.cc`).

## 4. Key Systems Breakdown

### Scripting Engine (`src/scripts.cc` & `src/js_integration.cc`)
Fallout 2 relies on scripts for almost all game logic. `fallout2-ce` supports two scripting backends concurrently:
- **SSL (`.int` files)**: The original legacy opcodes.
- **JavaScript (`.js` files)**: The modern QuickJS backend.

The `scripts` system manages a pool of active scripts, routing events (like "use object" or "talk to NPC") to the correct script, regardless of whether it is SSL or JS.

### The Virtual File System (`src/db.cc`)
As covered previously, this system reads the master `.dat` archives. When the engine needs a file (e.g., `art/critters/hmjmpsaa.frm`), it asks `db.cc`, which locates the file inside the archive, uncompresses it if necessary, and returns a handle.

### The Object System (`src/object.cc`)
Everything in the game world—critters, scenery, items, doors—is an "Object".
Objects have properties (coordinates, flags, inventory). When a map loads, the engine instantiates these objects into memory, manages their states, and cleans them up when the map is unloaded.

## Summary

The architecture of `fallout2-ce` is a faithful reconstruction of the original engine but modernised underneath to use SDL2 for IO and QuickJS for modern scripting.
In the next tutorial, we will move away from engine internals and look at how to create custom content: **Tutorial 3: Mapping and Tools**.
