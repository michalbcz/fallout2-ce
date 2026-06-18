# Tutorial 13: Adding Completely Custom Creatures

In Tutorial 8, we placed a Rat on our map. However, that rat was already part of the base game. What if you want to add an entirely new monster?

Adding a completely new creature involves three main steps: Art (.FRM), Configuration (.LST), and Prototype (.PRO).

## 1. Creating the Art (.FRM Files)

Fallout 2 sprites use a proprietary format called **.FRM**. A creature requires *many* animations (idle, walking, running, getting hit, dying in various ways, attacking), and each animation needs 6 directional facing angles.

**State-of-the-Art Free Tools:**
- **Blender (Free 3D):** The modern workflow involves modeling and animating the creature in Blender. You render the animation from a fixed isometric camera angle in all 6 directions.
- **Frame Animator / TitKit / FRM Maker:** You take your rendered PNG frames and use a community tool like "Frame Animator" (available on No Mutants Allowed) to pack them into .FRM files, ensuring the colors are constrained to the Fallout 8-bit palette.

Let's assume you've created your basic animations and saved them as:
`MAMONSAA.FRM` (Idle)
`MAMONSAB.FRM` (Walking)
...etc.

*Note: The naming convention is critical. `MA` denotes a monster, `MONS` is your custom 4-letter identifier, and `AA`/`AB` are the animation codes.*

Place these `.FRM` files into `data/art/critters/`.

## 2. Registering the Art (`critters.lst`)

The engine doesn't automatically detect your new `.FRM` files. You must register them.

1. Open `data/art/critters/critters.lst`.
2. Add your creature's 4-letter identifier (e.g., `MONS`) to the bottom of the list.
3. Note the line number where you placed it (e.g., if it's the 150th entry, its Art ID is 150).

## 3. Creating the Prototype (.PRO File)

A `.FRM` is just a picture. A Prototype (`.PRO`) tells the engine the creature's stats, HP, armor, and which art ID to use.

**Tool:** Use the **F2wedit** (Fallout 2 proto editor) or the built-in prototype editor in the **Community Mapper**.

1. Open the Mapper, go to the Critter prototypes tab.
2. Select an existing prototype that is similar to what you want (e.g., a Deathclaw) and click "Edit".
3. Change the **Art** to point to your new Art ID (from `critters.lst`).
4. Change the stats: increase HP, modify damage resistances, set the base Unarmed damage if it attacks with claws.
5. Save the prototype as a new file, e.g., `00000500.pro` in `data/proto/critters/`.
6. You also need to edit `data/text/english/pro_crit.msg` to give your new PRO file a name and description that the player sees when examining it.

## 4. Placing the Creature

Now, load your map in the Mapper. You will see your new custom creature in the Critters list. Place it on the map, assign it an AI script and Team Number (as covered in Tutorial 8), and your new monster is ready to terrorize the wasteland!
