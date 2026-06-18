# Tutorial 9: Adding an NPC with Dialogue and a Quest

Now we will create a friendly NPC who asks the player to fetch an item, and rewards them when they do. We will use the modern QuickJS scripting engine for this.

## 1. Placing the NPC

1. In the Mapper, place a human Critter on your map.
2. Open their properties and set their **Team** to a friendly team (like the player's team, or a custom peaceful town team).
3. Set their **Script** to a new script we will create: `MYMAYOR.JS`.

## 2. Creating the Message File (`.msg`)

Fallout separates text from logic to allow for translations. We need to create a message file for our NPC.

Create `data/text/english/dialog/mymayor.msg`:
```text
{100}{}{You see the Mayor of the new map.}
{101}{}{Hello traveler! I need a Stimpak. Can you bring me one?}
{102}{}{Sure, I'll find one.}
{103}{}{No way, I'm busy.}
{104}{}{Thank you! Here is 100 caps for your trouble.}
{105}{}{You already helped me. Thanks again!}
```

## 3. Defining the Quest Variable

Quests are tracked using Global Variables (GVARs) so they persist across maps.
Open `data/data/vault13.gam` (or your mod's equivalent variables file) and add:
```text
GVAR_MAYOR_QUEST_STATE := 0; // 0 = unassigned, 1 = active, 2 = completed
```

## 4. Writing the JavaScript Logic

Create `data/scripts/mymayor.js` and add it to `scripts.lst`.

```javascript
// mymayor.js

// Constants for our dialogue lines
const MSG_LOOK = 100;
const MSG_GREET = 101;
const MSG_ACCEPT = 102;
const MSG_DECLINE = 103;
const MSG_THANKS = 104;
const MSG_DONE = 105;

// Constants for items and variables (You would normally import these from a definitions file)
const PID_STIMPAK = 40;
const PID_BOTTLE_CAPS = 41;
const GVAR_MAYOR_QUEST = 500; // Assuming this is the ID we assigned in vault13.gam

export function look_at_p_proc() {
    fallout.displayMsg(fallout.mstr(MSG_LOOK));
}

export function talk_p_proc() {
    let questState = fallout.getGlobalVar(GVAR_MAYOR_QUEST);
    let dude = fallout.getDude();
    let self = fallout.getSelf();

    if (questState === 2) {
        // Quest is done
        fallout.floatMsg(self, fallout.mstr(MSG_DONE), 0);
        return;
    }

    if (questState === 1) {
        // Quest is active, check for Stimpak
        if (fallout.objIsCarryingObjPid(dude, PID_STIMPAK) > 0) {
            // Remove Stimpak
            let stim = fallout.objCarryingPidObj(dude, PID_STIMPAK);
            fallout.rmObjFromInven(dude, stim);
            fallout.destroyObject(stim);

            // Give caps
            let caps = fallout.createObject(PID_BOTTLE_CAPS, 0, 0);
            fallout.addMultObjsToInven(dude, caps, 100);

            // Update quest state
            fallout.setGlobalVar(GVAR_MAYOR_QUEST, 2);
            fallout.displayMsg("Quest Completed!");
            fallout.floatMsg(self, fallout.mstr(MSG_THANKS), 0);
        } else {
            fallout.floatMsg(self, "Did you find a Stimpak yet?", 0);
        }
        return;
    }

    // Quest is unassigned, initiate dialogue UI
    fallout.startGDialog(1000, self, 4, -1, -1);
    fallout.gsayStart();

    fallout.gsayReply(1000, MSG_GREET);
    fallout.giqOption(4, 1000, MSG_ACCEPT, node_accept, 50);
    fallout.giqOption(4, 1000, MSG_DECLINE, node_decline, 50);

    fallout.gsayEnd();
    fallout.endDialog();
}

function node_accept() {
    fallout.setGlobalVar(GVAR_MAYOR_QUEST, 1);
}

function node_decline() {
    // Player declined, do nothing. They can ask again later.
}
```

## 5. Summary

You have now created a complete gameplay loop!
1. The map is built.
2. The NPC is placed.
3. The `.msg` file contains their words.
4. The `.js` script drives the dialogue tree (`startGDialog`, `gsayReply`, `giqOption`), checks the player's inventory (`objIsCarryingObjPid`), modifies inventory (`createObject`, `rmObjFromInven`), and tracks progress using a Global Variable.

This pattern is the foundation for almost every quest in Fallout 2.
