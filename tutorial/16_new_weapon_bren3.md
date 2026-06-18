# Tutorial 16: Adding a New Weapon (CZ BREN 3)

Adding a custom weapon is one of the most rewarding modding tasks. In this tutorial, we will add the modern Czech CZ BREN 3 automatic rifle to the game.

## 1. Creating the Inventory Art (.FRM)

When you open your inventory, you see an icon for the weapon.
1. Find or draw a pixel-art representation of the CZ BREN 3. It must fit within the inventory grid (typically no larger than 90x60 pixels).
2. Convert it to the Fallout 8-bit palette using Frame Animator.
3. Save the file as `INV/BREN3.FRM` and place it in `data/art/inven/`.
4. Open `data/art/inven/inven.lst` and add `BREN3.FRM` to the list. Note the Art ID.

## 2. Defining the Item Prototype (.PRO file)

Every item in the game is defined by a Prototype (`.PRO`) file, which dictates its weight, damage, ammo type, and range.

1. Open **F2wedit** or the Mapper's Prototype Editor.
2. Go to the Items tab and open an existing assault rifle (like the Assault Rifle, PID 118) to use as a template.
3. Change the **Name** and **Description** string IDs (we will define these in `pro_item.msg` next).
4. Change the **Inventory Art** to the new ID you noted from `inven.lst`.
5. **Configure the Weapon Stats for the BREN 3:**
   - **Damage Type:** Normal (or Piercing if you prefer)
   - **Min Damage:** 15
   - **Max Damage:** 24
   - **Max Range:** 40
   - **Ammo Type:** 5.56mm (or create a custom ammo type if desired)
   - **Magazine Capacity:** 30
   - **Action Points:** 5 for single shot, 6 for burst.
   - **Anim Code:** Set this to `K` (which is the animation code for rifles/two-handed weapons).
6. Save the new Prototype (e.g., `00000600.pro`) to `data/proto/items/`.

## 3. Writing the Text Descriptions

The game needs to know what text to display when you hover over or examine the weapon.

1. Open `data/text/english/dialog/pro_item.msg`.
2. Scroll to the ID you assigned in the Prototype editor.
3. Add the text entries:
   ```text
   {60000}{}{CZ BREN 3}
   {60001}{}{A modern, highly modular assault rifle manufactured by Česká zbrojovka. It fires 5.56mm rounds and is extremely reliable in harsh wasteland conditions.}
   ```

## 4. Integrating into the Game World

You have defined the CZ BREN 3, but the player has no way to get it yet. You must add it to the world.

There are three main ways to do this:
1. **Mapper Placement:** Open a map in the Mapper and place the weapon on the ground or inside a locker.
2. **Merchant Inventories:** Edit the script of a merchant (e.g., `ZISMNTRD.INT` or a custom QuickJS merchant script) to dynamically add the weapon's PID to their inventory when the game loads.
3. **Loot Tables:** Edit the random encounter scripts so that certain high-level enemies have a chance to spawn with the weapon equipped.

## 5. Testing the Weapon

1. Load your game.
2. Obtain the weapon (either by picking it up where you placed it, or by writing a quick debug script `fallout.createObject(600, ...)`).
3. Open the inventory. You should see your custom BREN 3 icon.
4. Equip it. Your character will switch to the rifle animation stance (because you set the Anim Code to `K`).
5. Enter combat and test the burst fire to ensure the ammo depletion and damage ranges work correctly.

Congratulations! You have successfully added a completely new weapon to Fallout 2.
