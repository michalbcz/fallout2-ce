# Tutorial 01: Hello Fallout JS

In this tutorial, we will learn the basics of the Fallout 2 JavaScript API and how to print messages to the in-game display.

## The `fallout` Object

The engine provides a global `fallout` object that contains all the functions you need to interact with the game.

### The `print` and `displayMsg` Functions

There are two ways to show a message in the bottom-left window:
- `fallout.print(message)`
- `fallout.displayMsg(message)`

They do the exact same thing! `displayMsg` is often used by modders because it matches the original scripting language name.

## Script Procedures (Procs)

Fallout scripts are "Event Driven". This means the engine calls specific functions in your script when something happens in the game. These functions are called **procedures** or **procs**.

Common procs include:
- `start`: Called when the script is first loaded.
- `map_enter_p_proc`: Called when the player enters the map.
- `map_update_p_proc`: Called every few milliseconds while the player is on the map.

## Step-by-Step

1. Create a new script in `data/scripts/ARTemple.js` (or use the one from the previous tutorial).
2. Write the following code:

```javascript
/**
 * Tutorial 01: Hello World
 * This script runs on the first map of the game.
 */

function start() {
    // This goes to the terminal/log file
    console.log("ARTemple script has started!");
}

function map_enter_p_proc() {
    // This goes to the in-game monitor
    fallout.displayMsg("Welcome to the Temple of Trials, Chosen One.");
    fallout.displayMsg("JavaScript is watching you...");
}
```

3. Save the file and start a new game.
4. You will see your custom welcome message as soon as the map loads!

## Experiment!

Try changing the message or adding a `map_update_p_proc` (careful: this runs very often!).

---
**Next Step**: [Tutorial 02: Spatial Triggers](02_spatial_triggers.md)
