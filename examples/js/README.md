# Fallout 2 Community Edition JavaScript Examples

This directory contains simple examples of JavaScript scripts that can be run in Fallout 2 Community Edition.

## Examples

1.  `hello_world.js`: Prints "Hello, World!" to the game's debug log (stdout).
2.  `game_time.js`: Prints the current in-game time (hour) to the game's debug log.
3.  `display_msg.js`: Prints a message directly to the in-game display monitor (the text window at the bottom left).

## How to Run

1.  **Locate your Fallout 2 directory.** This is where you installed the game (e.g., `C:\GOG Games\Fallout 2` or `~/Fallout2`).
2.  **Copy the scripts.** Copy the `.js` files from this directory (`examples/js/`) into the `scripts/` subdirectory of your Fallout 2 installation.
    - If the `scripts/` directory doesn't exist, create it.
3.  **Update `scripts.lst`.**
    - Locate the `scripts/scripts.lst` file.
    - Open `scripts.lst` with a text editor.
    - Add the name of the script you want to run (without the extension) to the list.

### Testing on the Temple of Trials (Arroyo)

The easiest way to test these scripts immediately upon starting a new game is to replace the script for the starting map (Arroyo Temple).

1.  **Identify the Map Script.** The script for the Temple of Trials map is `artemple.int`.
2.  **Backup.** If `scripts/artemple.int` exists, rename it to `artemple.int.bak` so you can restore it later.
3.  **Prepare the JS Script.**
    - Copy one of the example scripts (e.g., `display_msg.js`) to `scripts/artemple.js`.
4.  **Update Script List.**
    - Open `scripts/scripts.lst`.
    - Find the line `artemple.int` (or just `artemple`).
    - If it says `artemple.int`, change it to `artemple.js`.
    - If it just says `artemple`, the engine will look for `.int` first, then `.js`. Ensure `artemple.int` is removed or renamed so the engine picks up `artemple.js`.
5.  **Run the Game.**
    - Start a new game.
    - As soon as the map loads (after the intro movie), the `start` function in `artemple.js` will execute.
    - If you used `display_msg.js`, you should see "Hello from the JS world! This message is on your monitor." in the message window.
    - If you used `hello_world.js`, check the console/terminal output for the log message.

## API

The JavaScript integration currently provides a global `fallout` object:

-   `fallout.gameTimeHour()`: Returns the current game hour (e.g., 1200 for 12:00 PM).
-   `fallout.print(message)`: Displays a message in the in-game display monitor.
-   `console.log(message)`: Prints a message to the game's debug log (stdout/stderr).
