# Tutorial 5: JavaScript Scripting

One of the most powerful features of `fallout2-ce` is its integration of **QuickJS**, allowing you to write game scripts in modern JavaScript (ES2020+). This allows for much cleaner syntax, better data structures, and an easier learning curve than legacy SSL.

## 1. Setup and Integration

JavaScript scripts are simply `.js` files. The engine recognizes them by their extension.
To add a JavaScript script to the game:
1. Write your `.js` file.
2. Place it in `data/scripts/`.
3. Add it to `data/scripts/scripts.lst` just like you would an `.int` file. The engine will automatically flag it with `SCRIPT_FLAG_JS` and route execution to the QuickJS context.

## 2. The `fallout` Object API

The engine exposes game functions to JavaScript through a global `fallout` object.

### Core Game Functions
- **Time:** `fallout.gameTime()`
- **Output:** `fallout.print(string)`, `fallout.displayMsg(string)`
- **Audio:** `fallout.playSound(soundName)`, `fallout.playMusic(musicName)`

### Object Identification
To interact with the world, you need to reference objects:
- `fallout.getSelf()`: Returns the object the script is attached to.
- `fallout.getSource()`: Returns the object that triggered the current event.
- `fallout.getTarget()`: Returns the object being acted upon.
- `fallout.getDude()`: Returns the player character.

### Variable Management
Just like SSL, JS scripts can access global and local state:
- `fallout.getGlobalVar(varId)` / `fallout.setGlobalVar(varId, value)`
- `fallout.getLocalVar(varId)` / `fallout.setLocalVar(varId, value)`

## 3. Event Handling in JavaScript

Instead of defining rigid procedures like `talk_p_proc`, in JavaScript you export specific functions that the engine will call when an event occurs.

Here is the JavaScript equivalent of the simple NPC script from the previous tutorial:

```javascript
// mynpc.js

export function start() {
    // Initialisation if needed
}

export function look_at_p_proc() {
    // We override default engine look behavior
    // Note: JS API might require calling an explicit override function depending on CE version
    fallout.displayMsg("You see a generic townsperson (JS).");
}

export function talk_p_proc() {
    let self = fallout.getSelf();
    // Assuming float_msg is exposed. Check examples/js/ for full API definitions.
    // fallout.floatMsg(self, "Hello there, traveler!", 0);
    fallout.print("The NPC says hello!");
}
```

## 4. Timers and State

JavaScript closures and modern syntax make complex logic much easier. However, remember that standard JS constructs like `setTimeout` do not align with the game engine's tick rate or save states.
Instead, you should use engine-provided timer events:
- `fallout.addTimerEvent(object, ticks, param)`
- `fallout.getFixedParam()` (used inside a `timed_event_p_proc` to read the param).

## 5. Further Reading

Because the JavaScript API in `fallout2-ce` is actively developed, you should always reference the `examples/js/` directory in the repository source code for the most up-to-date API bindings and advanced examples.

---
**Congratulations!** You now understand the C++ foundation of `fallout2-ce`, the engine architecture, mapping basics, and both legacy and modern scripting. Happy modding!
