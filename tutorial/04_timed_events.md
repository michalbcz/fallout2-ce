# Tutorial 04: Timed Events

In this tutorial, we will learn how to make NPCs do things periodically. This is useful for "ambient barks" (random text appearing over an NPC's head) or complex AI behaviors.

## What is a Timed Event?

A Timed Event is a "scheduled" message that the engine sends to a script after a certain amount of time.
- `fallout.addTimerEvent(delay, param)`: Schedules an event.
- `timed_event_p_proc()`: The function called when the time is up.

**Note on Time**: Fallout 2 uses "Game Ticks". There are **10 ticks in 1 second**. So, a delay of `50` is 5 seconds.

## Creating a "Loop"

To make an event happen repeatedly, you simply schedule the *next* event inside the `timed_event_p_proc` function.

## Step-by-Step: The Barking Guardian

Klint is the guardian at the entrance of the Temple of Trials. Let's make him remind the player to hurry up every 10 seconds.

1. Create a file named `data/scripts/ACKLINT.js`.
2. Write the following code:

```javascript
/**
 * Tutorial 04: Timed Events
 * NPC: Klint (Arroyo Temple Entrance)
 */

function map_enter_p_proc() {
    // Start the timer as soon as the map loads.
    // Event ID = 1, Delay = 100 ticks (10 seconds)
    fallout.addTimerEvent(100, 1);
}

function timed_event_p_proc() {
    // Check which event just fired (we used ID 1)
    let eventId = fallout.getFixedParam();

    if (eventId == 1) {
        // Make the NPC "bark" a message
        // (For now we use displayMsg, but in advanced tutorials
        // we will learn how to make text appear above their head!)
        fallout.displayMsg("Klint yells: 'Hurry up, Chosen One! The Elder is waiting!'");

        // Schedule the NEXT bark in 10 seconds to keep the loop going
        fallout.addTimerEvent(100, 1);
    }
}
```

3. Save the file.
4. Start a New Game.
5. Wait 10 seconds near Klint. You should see him "yelling" in the message window!

## Why use Event IDs?

You can have multiple timers running at once. By using different `param` values (e.g., 1 for barking, 2 for walking), you can distinguish them in `timed_event_p_proc`.

---
**Next Step**: [Tutorial 05: Creating a New Map](05_new_map.md)
