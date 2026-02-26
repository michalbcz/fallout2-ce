/**
 * Tutorial 02: Spatial Triggers
 *
 * This script demonstrates how to detect when the player walks over a trigger.
 * To use this, copy it to: data/scripts/ARBridge.js
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
