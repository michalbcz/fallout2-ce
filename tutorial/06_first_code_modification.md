# Tutorial 6: Your First Engine Modification

In this tutorial, you will make a simple, code-only modification to the `fallout2-ce` C++ engine, compile the changes, and see the result in-game. We will add a small cheat: making the player start with maximum Action Points (AP).

## 1. Locating the Code

To change how Action Points are calculated, we need to find the logic that handles stats. In the `fallout2-ce` source code, critter stats are generally managed in `src/stat.cc` or `src/critter.cc`.
Another easy modification is altering the starting game state. When a new game starts, the engine sets up the player character.

Let's do something very visual: let's modify the combat text so that whenever you hit an enemy, it always prints a custom debug message to the message box.

Open `src/combat.cc`.
Search for the function `combat_print_msg` or look for the combat loop `combat_attack`.

Alternatively, let's modify the Main Menu version string to leave our mark on the game!

1. Open `src/mainmenu.cc`.
2. Look for the `mainMenuRender` or `mainMenuWindowInit` functions where the version text is drawn.
3. You should see a call similar to `fontDrawText` or a string reference involving the version.

*For the sake of this tutorial, let's do a simple C++ print to the console when the game boots.*

1. Open `src/main.cc`.
2. Find the `falloutInit` function.
3. Right after the system starts up, add a logging print:

```cpp
#include "debug.h"

// inside falloutInit(int argc, char** argv)
debugPrint("Hello Wasteland! This is my first engine mod!\n");
```

## 2. Rebuilding the Project

`fallout2-ce` uses CMake. To build your changes, you must recompile the C++ code.

Open your terminal or command prompt in the root of the repository.

1. Configure the project (if you haven't already):
   ```bash
   cmake -B build -S .
   ```
2. Build the engine:
   ```bash
   cmake --build build -j$(nproc)
   ```

*(On Windows with Visual Studio, this might be `cmake --build build --config Release`).*

## 3. Testing Your Modification

1. Ensure your original Fallout 2 `master.dat` and `critter.dat` files are in the expected location (usually the same directory as the executable, or configured via `fallout2.cfg`).
2. Run the newly compiled executable:
   ```bash
   ./build/fallout2-ce
   ```
3. Look at your terminal or the `debug.log` file. You should see:
   ```text
   Hello Wasteland! This is my first engine mod!
   ```

## 4. Why This Matters

This simple change demonstrates the core loop of engine modding:
1. Locate the logic in the `src/` directory.
2. Make your C++ changes.
3. Recompile using CMake.
4. Run the executable to test.

Next, we will move away from C++ and look at game data modding in **Tutorial 7: Adding a New Map**.
