/**
 * Tutorial 01: Hello World
 *
 * This is a simple script that prints a message when you enter the map.
 * To use this, copy it to: data/scripts/ARTemple.js
 */

function start() {
    // console.log prints to the debug log (js_debug.log or terminal)
    console.log("ARTemple script has started!");
}

function map_enter_p_proc() {
    // fallout.displayMsg prints to the in-game text window (bottom-left)
    fallout.displayMsg("Welcome to the Temple of Trials, Chosen One.");
    fallout.displayMsg("JavaScript is watching you...");
}
