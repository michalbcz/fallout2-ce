# Fallout 2 Community Edition JavaScript Examples

This directory contains simple examples of JavaScript scripts that can be run in Fallout 2 Community Edition.

## Examples

1. `hello_world.js`: Prints "Hello, World!" to the debug console.
2. `game_time.js`: Prints the current in-game time (hour) to the debug console.

## How to Run

1.  **Locate your Fallout 2 directory.** This is where you installed the game (e.g., `C:\GOG Games\Fallout 2` or `~/Fallout2`).
2.  **Copy the scripts.** Copy the `.js` files from this directory (`examples/js/`) into the `scripts/` subdirectory of your Fallout 2 installation.
    - If the `scripts/` directory doesn't exist, create it.
3.  **Update `scripts.lst`.**
    - Locate the `scripts/scripts.lst` file. If it doesn't exist, you may need to extract it from `master.dat` or create a new one (though the game usually expects one to exist if you are adding new scripts).
    - Open `scripts.lst` with a text editor.
    - Add the name of the script you want to run (without the extension) to the list.
    - **Note:** To actually *trigger* these scripts in-game, you would typically replace an existing script attached to an object or map.
    - For testing purposes without replacing game logic, you can temporarily rename one of these scripts to match an existing script name (e.g., `obj_dude.js` if you want it to run on the player character, though `obj_dude` is complex).
    - A safer way to test is to use a map script or a specific item script that you can easily trigger.

    **Example (replacing a map script):**
    If you are in Arroyo, the map script might be `arroyo.int`.
    1. Backup `scripts/arroyo.int`.
    2. Copy `examples/js/hello_world.js` to `scripts/arroyo.js`.
    3. Ensure `arroyo.int` is *not* in the `scripts/` folder (or just rely on the engine preferring one, but clearer to replace).
    4. Start the game and enter Arroyo. The `start` function should execute (depending on how the map script is invoked).

## API

The JavaScript integration currently provides a global `fallout` object:

-   `fallout.gameTimeHour()`: Returns the current game hour (e.g., 1200 for 12:00 PM).
-   `console.log(message)`: Prints a message to the game's debug log.
