/**
 * Tutorial 05: New Map Script
 *
 * This is a basic map script for a custom map.
 * To use this:
 * 1. Create a map in the BIS Mapper.
 * 2. Set the map script to 'MyMapScript'.
 * 3. Save this file as 'data/scripts/MyMapScript.js'.
 * 4. Add 'MyMapScript.js' to the end of scripts.lst.
 */

function map_enter_p_proc() {
    fallout.displayMsg("Welcome to your custom map! This is running in JavaScript.");

    // Check if it's the first time entering
    // (Map scripts don't have LVARs like objects do,
    // but you can use GVARs or even JS global variables if the script is never unloaded!)
    console.log("Custom map loaded successfully.");
}

function map_update_p_proc() {
    // This runs every game tick
}
