# Tutorial 03: Variables and State

Often, you want your scripts to "remember" things. For example, you might want to show a different message the third time a player walks over a trigger.

## Types of Variables

Fallout 2 has two main types of persistent variables:

### 1. Local Variables (LVARs)
These are variables specific to **one script**. If you have two doors with the same script, they each have their own set of Local Variables.
- `fallout.getLocalVar(index)`
- `fallout.setLocalVar(index, value)`

### 2. Global Variables (GVARs)
These are variables shared by the **entire game**. They are used to track quest progress, reputation, or major world events.
- `fallout.getGlobalVar(index)`
- `fallout.setGlobalVar(index, value)`

## Using Local Variables

Every script in Fallout 2 must declare how many local variables it uses. When we override an existing script like `ARBridge`, we can use the variables already allocated for it.

The original `ARBridge` script has **6 local variables** (indexed 0 to 5).

## Step-by-Step: Counting Bridge Crossings

Let's modify our Bridge script to count how many times the player has stepped on it.

1. Open `data/scripts/ARBridge.js`.
2. Update the code:

```javascript
/**
 * Tutorial 03: Variables and State
 * Location: Arroyo Bridge
 */

function spatial_p_proc() {
    if (fallout.getSource() == fallout.getDude()) {

        // Read the current count from Local Variable 0
        let count = fallout.getLocalVar(0);

        // Increment the count
        count = count + 1;

        // Save it back!
        fallout.setLocalVar(0, count);

        if (count == 1) {
            fallout.displayMsg("You step onto the bridge for the first time.");
        } else if (count == 5) {
            fallout.displayMsg("You've walked over this bridge 5 times now. Do you have nowhere else to go?");
        } else {
            fallout.displayMsg("You have crossed this bridge " + count + " times.");
        }
    }
}
```

3. Save and test. Notice how the game remembers the `count` even if you save and load the game!

## Where are these stored?
- **LVARs** are saved inside the map file (`.sav`) for that specific object.
- **GVARs** are saved in the main save file (`SAVEGAME.GAM`).

---
**Next Step**: [Tutorial 04: Timed Events](04_timed_events.md)
