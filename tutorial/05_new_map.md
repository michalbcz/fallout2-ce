# Tutorial 05: Creating a New Map

In this final tutorial, we move beyond overriding existing scripts and learn how to create an entirely new location in the Fallout 2 world using JavaScript.

## 1. Modding Tools

To create a new map, you need the **Official BIS Mapper**. Since it is a legacy 32-bit application, it is recommended to use a version patched for modern Windows.

### Where to get the Mapper:
- Search for the "Fallout 2 Mapper" on [No Mutants Allowed (NMA)](https://www.nma-fallout.com/).
- The [Vault-Tec Labs Wiki](https://falloutmods.fandom.com/wiki/Fallout_2_Mapper) has a comprehensive guide on installation.

## 2. Creating the Map

1. Launch the Mapper.
2. Paint some floor tiles and place a few objects.
3. Save your map as `MyFirstMap.map` in the `data/maps/` directory of your Fallout 2 installation.

## 3. Registering the Map

The engine doesn't automatically know about your new `.map` file. You must register it in `data/data/maps.txt`.

1. Open `data/data/maps.txt` (you may need to extract it from `master.dat` using a DAT explorer if it's not in your `data` folder).
2. Scroll to the bottom and add a new entry:

```ini
[Map 150]
map_name=MyFirstMap
lookup_name=My First JavaScript Map
music=07desert
exit_grids=0
type=1
```
*(Note: Use the next available Map number; 150 is just an example.)*

## 4. Creating the Map Script

Now, let's create a script that runs specifically for this map.

1. Create a file named `data/scripts/MyMapScript.js`.
2. Add this code:

```javascript
function map_enter_p_proc() {
    fallout.displayMsg("Welcome to your custom map! This is running in JavaScript.");
}
```

3. Open `data/scripts/scripts.lst` and add your script to the end of the file:
   `MyMapScript.js  # local_vars=0`
   **CRITICAL**: Do not remove any existing lines from `scripts.lst`, just append to the end.

## 5. Linking Script to Map

1. Go back to the **BIS Mapper**.
2. Open your `MyFirstMap.map`.
3. In the **Map Header** settings (usually under Map -> Header), set the **Script** to `MyMapScript`.
4. Save the map.

## 6. Teleporting to your Map

To test it, you can tell the engine to start directly on your new map. Fallout 2 CE supports a subset of `sfall` settings via a file named `ddraw.ini`.

1. Look for `ddraw.ini` in your game folder. If it doesn't exist, create a new text file and rename it to `ddraw.ini`.
2. Open it and ensure it has a `[Misc]` section (not `[Main]`) with the `StartingMap` setting:

```ini
[Misc]
StartingMap=MyFirstMap.map
```

3. Run the game and start a **New Game**. Instead of the Temple of Trials, you should load directly into your new map!

Congratulations! You have created a new location and attached a JavaScript script to it.

## Where to go from here?

You now have the basics of Fallout 2 JS modding. You can:
- Create NPCs with complex dialogue.
- Build entire new quests using Global Variables.
- Use `addTimerEvent` to create living, breathing environments.

For a full list of supported functions, check the `src/js_integration.cc` file in the engine source code.

Happy modding!
