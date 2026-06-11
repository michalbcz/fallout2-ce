# Tutorial 3: Mapping and Tools

Creating custom maps is one of the most exciting ways to mod Fallout 2. Because `fallout2-ce` uses the original engine formats, the process for creating maps is identical to classic Fallout 2 modding.

## 1. Choosing the Right Tool

While Black Isle Studios released an official mapper decades ago, the modern community standard is the **Fallout 2 Community Mapper** (often bundled or associated with sfall).
It features numerous quality-of-life improvements, bug fixes, and better compatibility with modern operating systems.

### Where to get it
You can generally find the Community Mapper on platforms like No Mutants Allowed (NMA) or the sfall GitHub repository. Ensure you configure its `mapper2.cfg` file to point to your Fallout 2 installation directory, so it can load the base game's art and prototypes.

## 2. Basic Mapping Concepts

A Fallout map is comprised of several layers:
- **Tiles (Hexes):** The floor. Fallout 2 uses an isometric hex grid. You "paint" floor tiles (like dirt, metal plating, or grass).
- **Roofs:** Similar to tiles, but they block line of sight and are hidden when the player walks under them.
- **Scenery:** Walls, trees, tables. These block movement or line of sight.
- **Objects/Items:** Things the player can pick up (stimpaks, guns).
- **Critters:** NPCs and monsters.
- **Spatial Scripts:** Invisible hexes that trigger a script when the player steps on them (e.g., triggering an ambush or a trap).

## 3. Step-by-Step Workflow

### Step 1: Create a New Map
1. Open the Mapper tool.
2. Select File -> New.
3. You will be presented with an empty void. First, select the "Tiles" inventory and begin painting a floor layout to establish the walkable area.

### Step 2: Build Structures
1. Switch to the "Scenery" tab.
2. Place wall pieces. Pay close attention to corners and intersections—isometric walls require specific pieces to look correct.
3. Place doors, making sure they align with the walls.

### Step 3: Populate the Map
1. Place **Critters** (NPCs, enemies). For each critter, you can assign a Script ID. This script will dictate their dialogue and AI behavior (which we cover in the next tutorials).
2. Place **Items** on the ground or inside containers (like lockers or desks).

### Step 4: Map Entrances and Exits
To make the map functional, the player needs to be able to arrive and leave:
- Place an **Exit Grid** (found in Scenery/Misc). This allows the player to leave the map and return to the World Map.
- Set up **Spawn Points** so the engine knows where the player should appear when they enter the map.

### Step 5: Save and Integrate
1. Save the file (e.g., `MYMAP.MAP`).
2. Move `MYMAP.MAP` into your `fallout2-ce/data/MAPS/` directory.
3. Update the game's `data/data/maps.txt` (or modify `city.txt`) so the game engine acknowledges the new map exists and can load it.

## 4. Testing Your Map

To test the map in `fallout2-ce`:
You can write a simple debug script to teleport the player to the map, or temporarily modify your start location in `data/data/maps.txt`.
Run `fallout2-ce` and navigate to your map to ensure pathfinding, walls, and objects behave as expected.

---
In the next tutorial, **Tutorial 4: SSL Scripting**, we will learn how to breathe life into the static NPCs and objects you just placed.
