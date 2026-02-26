# Tutorial 02: Spatial Triggers

In this tutorial, we will learn about "Spatial Scripts". These are scripts triggered when a character walks over a specific hex on the map.

## Spatial Procedures

When a character walks over a spatial trigger, the engine calls the `spatial_p_proc` function in the script attached to that hex.

### Who triggered it?

To make interesting triggers, we often need to know who walked over it.
- `fallout.getDude()`: Returns the Player character.
- `fallout.getSource()`: Returns the object that triggered the script (the "Source").

### Comparing Objects

You can compare game objects using `==` in JavaScript. For example:
`if (fallout.getSource() == fallout.getDude())` checks if the player is the one who stepped on the trigger.

## Step-by-Step: The Arroyo Bridge

The Great Bridge in Arroyo (the map where you start after the Temple) has spatial triggers at both ends. We can override the bridge script to add our own messages.

1. Create a file named `data/scripts/ARBridge.js`.
2. Write the following code:

```javascript
/**
 * Tutorial 02: Spatial Triggers
 * Location: Arroyo Bridge
 */

function spatial_p_proc() {
    // Get the character who stepped on the trigger
    let source = fallout.getSource();
    let dude = fallout.getDude();

    // Only show the message if the PLAYER stepped on it
    if (source == dude) {
        fallout.displayMsg("The bridge creaks under your weight.");
    } else {
        // This might happen if an NPC or a dog walks over it!
        console.log("Something else stepped on the bridge...");
    }
}
```

3. Save the file.
4. Go to the Arroyo Bridge map (exit the Temple and go south).
5. Walk onto the bridge. You should see "The bridge creaks under your weight." in the message window.

## Using `getSelf()`

Spatial scripts are usually "invisible" objects. You can use `fallout.getSelf()` to get the object representing the trigger itself, though it's less common for spatial scripts than for NPCs.

---
**Next Step**: [Tutorial 03: Variables and State](03_variables.md)
