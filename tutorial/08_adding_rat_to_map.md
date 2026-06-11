# Tutorial 8: Adding a Rat to the Map

An empty map is boring. Let's add a hostile Rat to your new map. We will use the Mapper tool to place the object and assign it the correct script so it attacks the player.

## 1. Placing the Critter

1. Open `MYMAP.MAP` in the Community Mapper.
2. In the bottom interface, click on the **Critters** button.
3. Scroll through the prototypes until you find a standard "Rat" or "Mole Rat".
4. Click on the Rat prototype, then click somewhere on your map to place it.

## 2. Assigning the Script

A critter placed on a map is just a statue unless it has a script assigned to tell the engine how it should behave.

1. Double-click the Rat you just placed on the map. This opens the **Object Properties** window.
2. Look for the field labeled **Script**.
3. In Fallout 2, standard generic rats use a script named `ZCRAT.INT`.
4. Click the Script dropdown/button and select `ZCRAT` from the list.

*(If you were making a custom boss rat, you would write `mybossrat.js` and select that instead, but for standard enemies, we reuse base game scripts).*

## 3. Team Numbers and AI Packets

Scripts control dialogue and complex logic, but basic combat behavior (who to attack, when to flee) is handled by the **AI Packet** and **Team Number**.

While still in the Object Properties window for the Rat:
1. **Team Number:** Set this to a number corresponding to "Generic Hostile Mutants" or "Rats" (often Team 3 or similar, check existing maps for reference). Any critter not on Team 0 (the player's team) will be considered hostile by default if their script dictates it.
2. **AI Packet:** Set this to `AI_RAT` or `AI_GENERIC_RAT`. This tells the engine that the critter should fight using melee, run up to the player, and flee if its HP drops below 20%.

## 4. Save and Test

1. Save `MYMAP.MAP`.
2. Load up `fallout2-ce` and travel to your map.
3. As you approach the Rat, the `ZCRAT` script will detect your proximity, verify you are not on the same team, and initiate combat based on its AI packet.

In the next tutorial, we will do something much more complex: adding a friendly NPC, giving them dialogue, and creating a quest using JavaScript.
