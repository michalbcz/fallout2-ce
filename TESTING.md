# Testing and Debugging Features

This document analyzes the existing debug and testing features present in the Fallout 2 Community Edition engine, and proposes new features to facilitate manual testing.

## Existing Debug/Test Build Features

The engine contains several built-in mechanisms to aid in testing and debugging.

### Debug Build Macros
* **`NDEBUG` macro**: In the codebase, specifically in `src/debug.cc`, the `debugPrint` function relies on the absence of the `NDEBUG` macro to output logs directly to the console via `SDL_LogMessageV`. If `NDEBUG` is defined (which is typical for Release builds), `debugPrint` calls that do not have a registered log handler will be silently ignored. Thus, compiling the game as a Debug build (without `NDEBUG`) enables console output by default.

### `fallout2.cfg` Configuration
The game configuration file allows defining a `[debug]` section with several keys that alter the engine's behavior:

* **`mode`**: Determines how debug output is handled. Setting it to `environment`, `screen`, `log`, `mono`, or `gnw` registers different handlers for `debugPrint`. For instance, `mode=log` will output all debug prints to `debug.log`. This can also be controlled via the `DEBUGACTIVE` environment variable.
* **`show_script_messages`**: When set to `1`, script messages (such as `display_msg` opcodes) are duplicated to the debug output. This is useful for tracking script flow without watching the in-game display monitor.
* **`show_load_info`**: Enables logging of additional file load information.
* **`show_tile_num`**: When enabled, the engine can visually display tile numbers or log tile information, which is useful for mapper debugging or pathfinding logic verification.
* **`output_map_data_info`**: Outputs additional map data logs.

### Sound Debugging
* The `[sound]` section in `fallout2.cfg` contains a `debug` flag which can be used to enable audio-related debug prints (e.g. initialization errors, device setup).

---

## Proposed Features for Manual Testing

To make manual testing of Fallout 2 as efficient as possible for a QA tester or developer, the following "cheat" and quality-of-life debugging features are proposed:

### 1. Invincibility (God Mode)
A toggleable mode where the player character takes zero damage from any source (combat, environment, traps) and is immune to status effects like radiation or poison. This allows testing dangerous areas without the risk of dying and losing progress.

### 2. Superfast Movement (Speedhack)
A toggleable hotkey to multiply the player's movement and animation speed (e.g., 3x or 5x speed). This drastically reduces the time spent walking across large maps or waiting for long animations to finish during testing.

### 3. Item Summoning (Spawn Menu)
An accessible in-game developer menu or console command to spawn any item in the game directly into the player's inventory by its ID or name. This allows testing specific weapon interactions, quest items, or armor sets without having to progress through the entire game to find them.

### 4. "Kill Stare" (Insta-kill Hotkey)
A "magic button" that instantly kills any character the player's cursor is pointing at, without triggering aggro from surrounding NPCs or leaving a trace that the player caused it (no reputation or karma loss). This is invaluable for bypassing long, unwanted combat encounters or testing post-death states of specific NPCs.

### 5. Annotated Dialog Choices
When talking to NPCs, dialog options should be visually annotated if they are gated behind specific game states (e.g., `[Requires INT > 7]`, `[Requires Quest 'Find the GECK' active]`, or `[Requires Speech > 50]`). Furthermore, a debug toggle should allow the tester to **see all hidden dialog choices** regardless of whether they meet the current requirements, making it easier to test dialog trees comprehensively.

### 6. Free Teleportation
A hotkey that instantly teleports the player character to the current cursor location on the map. Additionally, a world map menu to teleport instantly to any known or unknown city/location without spending game time or triggering random encounters.

### 7. In-Game Character Editor
A real-time menu to freely edit the player's stats, skills, traits, and perks during gameplay without restarting or using an external save editor. This allows testing how different character builds interact with the game world and skill checks on the fly.