/**
 * Tutorial 03: Variables and State
 *
 * This script demonstrates how to use Local Variables to keep track of state.
 * To use this, copy it to: data/scripts/ARBridge.js
 */

function spatial_p_proc() {
    // Only respond to the player
    if (fallout.getSource() == fallout.getDude()) {

        // Read the current count from Local Variable 0
        // ARBridge has 6 local variables allocated (0-5).
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
