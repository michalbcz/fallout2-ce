# Tutorial 00: Getting Started with Fallout 2 JS Modding

Welcome! This guide will help you set up your development environment to start modding Fallout 2 using JavaScript.

## 1. Prerequisites

### Game Assets
You must own a copy of Fallout 2. You can get it from:
- [GOG](https://www.gog.com/game/fallout_2)
- [Steam](https://store.steampowered.com/app/38410)
- [Epic Games Store](https://store.epicgames.com/p/fallout-2)

### Building the Engine
Fallout 2 Community Edition (CE) is an open-source engine that runs on modern systems. To follow these tutorials, you should build the engine from source to ensure you have the latest JavaScript features.

Refer to the [CONTRIBUTING.md](../CONTRIBUTING.md) file in the root of this repository for detailed build instructions for your platform:
- **Windows**: Use `build_windows.bat`.
- **Linux/macOS**: Use the provided CMake presets.

Once built, you will have an executable (e.g., `fallout2-ce.exe` on Windows).

## 2. Environment Setup

### Create a "Modding" Folder
Instead of modding your original game directory directly, it's best to create a working copy:
1. Create a new folder (e.g., `C:\Games\Fallout2-JS`).
2. Copy the following files from your original Fallout 2 installation into this folder:
   - `master.dat`
   - `critter.dat`
   - `patch000.dat` (if present)
   - `data/` folder (entire directory)
3. Copy your newly built `fallout2-ce` executable into this folder.

### Preparing the Scripts Folder
The engine looks for JavaScript scripts in `data/scripts/`.
1. Inside your new folder, ensure the `data/scripts/` directory exists.
2. This is where you will place your `.js` files.

## 3. How JavaScript Scripting Works

In Fallout 2 CE, you can replace any game script with a JavaScript file. You don't need to compile them; the engine reads the `.js` files directly!

The engine uses "Auto-Detection":
- If the engine wants to run a script named `ARTEMPLE` (the Temple of Trials map script), it first checks if `data/scripts/ARTEMPLE.js` exists.
- If it finds the `.js` file, it runs it instead of the original `.int` script.

## 4. Your First Test

To verify everything is set up correctly:
1. Create a file named `data/scripts/ARTemple.js`.
2. Open it in a text editor (like VS Code or Notepad++) and paste this:

```javascript
function map_enter_p_proc() {
    fallout.print("JavaScript Modding System: ONLINE");
}
```

3. Launch `fallout2-ce`.
4. Start a **New Game**.
5. Once you are in the Temple of Trials, look at the bottom-left text window. You should see "JavaScript Modding System: ONLINE".

---
**Next Step**: [Tutorial 01: Hello Fallout JS](01_hello_world.md)
