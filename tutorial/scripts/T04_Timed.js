/**
 * Tutorial 04: Timed Events
 *
 * This script makes Klint (at the Temple entrance) yell periodically.
 * To use this, copy it to: data/scripts/ACKLINT.js
 */

function map_enter_p_proc() {
    // Start the timer as soon as the map loads.
    // Delay = 100 ticks (10 seconds), Param = 1
    console.log("Starting Klint's timer...");
    fallout.addTimerEvent(100, 1);
}

function timed_event_p_proc() {
    // fallout.getFixedParam() returns the 'param' we passed to addTimerEvent
    let eventId = fallout.getFixedParam();

    if (eventId == 1) {
        // Show a message
        fallout.displayMsg("Klint yells: 'Hurry up, Chosen One! The Elder is waiting!'");

        // Schedule the NEXT bark in 10 seconds to keep the loop going
        fallout.addTimerEvent(100, 1);
    }
}
