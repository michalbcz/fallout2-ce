# Fallout 2 Community Edition — JavaScript Scripting

This directory contains example JavaScript scripts for Fallout 2 CE's QuickJS integration.

## Example Scripts

| File | Description |
|------|-------------|
| `hello_world.js` | Prints "Hello, World!" via `console.log`. Only visible when running from a terminal. |
| `game_time.js` | Prints the current in-game hour using `fallout.gameTimeHour()`. |
| `display_msg.js` | Shows a message in the in-game monitor (bottom-left text area). **Best script for testing.** |

---

## How JS Scripts Work

### Script discovery (auto-detection)

Since CE builds with this feature, you **do not need to edit `scripts.lst`** to use a JS script. The engine now automatically checks for a `.js` file on disk whenever it is about to load any script. If found, it takes precedence over the compiled `.int` from `master.dat`.

The engine probes these paths (in order) for every script that is about to run:

```
data/scripts/<name>.js       ← preferred location (matches VFS patches dir)
data\scripts\<name>.js       ← same, Windows backslash variant
scripts/<name>.js
scripts\<name>.js
```

The first path that resolves to a real file wins. The `.int` from `master.dat` is only used if no `.js` file is found.

### Script procedure names

JS scripts must export functions matching the standard Fallout 2 script procedure names. The engine calls these by name:

| Procedure | When called |
|-----------|-------------|
| `start` | Script first loaded |
| `map_enter_p_proc` | Player enters the map |
| `map_exit_p_proc` | Player leaves the map |
| `map_update_p_proc` | Every game tick while on map |
| `critter_p_proc` | Critter AI tick |
| `timed_event_p_proc` | Timed event fires |

Your script only needs to define the procedures it uses. Missing procedures are silently skipped.

---

## Quick Start — Temple of Trials

The Temple of Trials is the first map in the game, making it ideal for testing.

### Step 1 — Copy your JS script

Copy the example script into your game's patches scripts folder:

```
data\scripts\ARTemple.js
```

For example, to test the in-game message display:

```
copy examples\js\display_msg.js  "C:\<your game dir>\data\scripts\ARTemple.js"
```

> **Note:** The filename must match the script name from `scripts.lst` — for the Temple of Trials this is `ARTemple` (case-insensitive on Windows). The compiled INT for all other scripts continues to work unchanged.

### Step 2 — Launch the game

Run `fallout2-ce.exe` from the game's root directory. Start a new game.

The engine will:
1. Load `scripts.lst` from `data\scripts\scripts.lst` if it exists, otherwise from `master.dat`
2. Detect that `data\scripts\ARTemple.js` exists on disk
3. Override the scripts.lst `.int` entry and run your JS instead

### Step 3 — Verify it worked

- **In-game:** If using `display_msg.js`, you'll see *"Hello from the JS world! This message is on your monitor."* in the bottom-left message window when the Temple of Trials loads.
- **Log file:** A `js_debug.log` file is always written to the game directory. It contains detailed trace of every script loaded and executed. Look for:
  ```
  -> AUTO-DETECT: Found JS file on disk: 'data\scripts\ARTemple.js' — overriding to JS!
  -> Loading JS script: 'ARTemple.js'
  --- JS file first lines ---
  1: function start() {
  ...
  ```

---

## JavaScript API

The engine exposes a global `fallout` object and a `console` object:

### `fallout` object

```js
fallout.print(message)
```
Displays `message` in the in-game monitor (the scrolling text window, bottom-left of the screen).

```js
fallout.gameTimeHour()
```
Returns the current game time as an integer in military format (e.g. `1430` = 2:30 PM).

### `console` object

```js
console.log(message)
```
Writes `message` to the debug log (`debugPrint`). Visible in the terminal if the game is run from one, or via the `debug.log` file if `[debug] mode=log` is set in `fallout2.cfg`.

### Example script

```js
function map_enter_p_proc() {
    fallout.print("Map entered at hour: " + fallout.gameTimeHour());
}

function start() {
    fallout.print("Script loaded!");
}
```

---

## Debug Logging

The engine always writes `js_debug.log` to the game directory. It records:

- Which `scripts.lst` file was opened and how many entries it parsed
- Every `scriptExecProc` call — script index, entry name from `scripts.lst`, and whether `isJs` is true
- Auto-detection probes — which paths were checked for `.js` files and which was found
- For JS scripts: the exact file that was read and its first 5 lines
- For INT scripts: the full resolved path passed to the interpreter
- JS evaluation errors and runtime exceptions with full messages

This file is invaluable for diagnosing why a script is or is not running.

---

## Known Limitations

- **Each JS script has its own isolated QuickJS context.** Scripts cannot share global variables with each other.
- **JS file loading uses `fopen()` with relative paths**, not the engine's VFS `fileOpen()`. This means the game must be launched from the game root directory (the one containing `data\`).
- **Error messages from JS eval/runtime appear in `js_debug.log`** and the in-game monitor (for critical failures), but not in the game's built-in script debug overlay.
- **`scripts.lst` line order must never change.** Map files reference scripts by line index. If you put a custom `scripts.lst` in `data\scripts\`, keep all 1303 lines intact and only change extensions.

---

## How `scripts.lst` Integration Works

`scripts.lst` is read from the VFS. Files in the `data\` patches directory take precedence over `master.dat`. The typical resolution order is:

1. `data\scripts\scripts.lst` (loose file — wins if present)
2. `scripts\SCRIPTS.LST` inside `master.dat` (built-in fallback)

In vanilla Fallout 2, `artemple` is at **line index 744** (0-based) in `scripts.lst`. The `artemple.map` file stores `scriptIndex=745` (1-based). The CE auto-detection feature means you never need to change this — just drop a `.js` file in `data\scripts\`.
