# Tutorial 7: Adding a New Map

In Tutorial 3, we discussed the Mapper tool. Now, let's actually integrate a brand new map into the game engine so the player can visit it.

## 1. Exporting the Map

Assuming you have created a basic map in the Mapper tool (with a floor, an exit grid, and a spawn point):
1. Save the map as `MYMAP.MAP`.
2. Move `MYMAP.MAP` into the game directory: `data/maps/MYMAP.MAP`.

## 2. Registering the Map in `maps.txt`

The game engine does not scan the maps folder; you must explicitly tell it that the map exists. This is done via `data/data/maps.txt`.

1. Open `data/data/maps.txt` in a text editor.
2. Scroll to the bottom of the list. You will see entries like:
   ```text
   map_149=random_enc_desert_9.map
   ```
3. Add your map at the next available index:
   ```text
   map_150=MYMAP.MAP
   ```
*(Note: The exact number depends on your installed mods, just use the next number in the sequence).*

## 3. Adding the Map to a City (`city.txt`)

To make the map appear on the World Map as a green circle you can travel to, you must define it as a city or part of a city in `data/data/city.txt`.

1. Open `data/data/city.txt`.
2. Find the `[Area XX]` blocks. Create a new one at the end of the file:
   ```text
   [Area 50]
   area_name=My New Town
   world_pos=450, 450
   start_state=1
   lock_state=0
   size=small
   townmap_art_idx=-1
   townmap_label_art_idx=-1
   entrance_0=MyMap, 0, 0
   ```

   - `world_pos` dictates where on the global map it appears.
   - `entrance_0` maps to the internal name we gave it. It follows the format: `MapName, Elevation, StartTile`.

## 4. Alternative: Teleporting via Script

If you don't want to place it on the world map yet and just want to test it, you can create a small JavaScript cheat script to teleport the player.

Create `data/scripts/cheat.js`:
```javascript
export function start() {
    // 150 is the ID we gave it in maps.txt
    fallout.loadMap(150);
}
```
You can bind this script to an item in the game (like a Stimpak) so that when you use the item, you are instantly teleported to your new map.

## 5. Summary
You have successfully added a map to the game! However, right now it is empty. In **Tutorial 8: Adding a Rat to the Map**, we will add our first hostile encounter.
