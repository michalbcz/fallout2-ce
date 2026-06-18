# Tutorial 4: SSL Scripting (StarLoki Scripting Language)

SSL (StarLoki Scripting Language) is the original scripting language used in Fallout 1 and 2. It is a compiled language: you write source code (`.ssl`), and compile it into an intermediate byte code (`.int`) which the engine runs.

While `fallout2-ce` supports modern JavaScript, understanding SSL is essential because the vast majority of the original game's logic is written in it.

## 1. Syntax Basics

SSL looks a bit like Pascal. Statements are terminated with semicolons. Blocks of code are wrapped in `begin` and `end`.

```ssl
procedure start begin
    // Your code here
end
```

### Variables
There are three main types of variables in SSL:
- **Local Variables (`LVAR`)**: Unique to the specific instance of the object/critter running the script.
- **Map Variables (`MVAR`)**: Shared among all scripts running on the current map.
- **Global Variables (`GVAR`)**: Game-wide state variables (used for quest progress, reputation, etc.).

## 2. Event-Driven Procedures

SSL is heavily event-driven. The engine calls specific procedures based on what happens in the game world.

Here are the most common procedures you must implement for an NPC:

- `start`: Called when the script is loaded. Usually used for initialisation.
- `critter_p_proc`: Called every game tick while the critter is on the map. Used for AI and movement.
- `talk_p_proc`: Called when the player clicks on the NPC to talk to them.
- `destroy_p_proc`: Called when the NPC dies. Used to lower town reputation, drop items, or set quest states.
- `look_at_p_proc`: Called when the player hovers their mouse over the NPC.
- `description_p_proc`: Called when the player uses the "examine" binoculars icon on the NPC.

## 3. A Simple NPC Script

Here is an example of a very basic NPC that gives a greeting when talked to.

```ssl
#include "define.h"
#include "command.h"

// Standard setup macros
#define NAME                    SCRIPT_MYNPC
#define TOWN_REP_VAR            GVAR_TOWN_REP_MYTOWN

procedure start;
procedure talk_p_proc;
procedure look_at_p_proc;

procedure start begin
end

procedure look_at_p_proc begin
    script_overrides;
    display_msg("You see a generic townsperson.");
end

procedure talk_p_proc begin
    float_msg(self_obj, "Hello there, traveler!", FLOAT_MSG_NORMAL);
end
```

## 4. Compilation

To compile an `.ssl` file into an `.int` file, you need the **sfall script compiler** (compile.exe).
You will run a command like:
```cmd
compile.exe myscript.ssl myscript.int
```
You then place `myscript.int` into the `data/scripts/` folder, and add an entry for it in `data/scripts/scripts.lst`.

## 5. Summary

SSL is simple but rigid. You are limited to the opcodes built into the engine. For more complex modding, `fallout2-ce` provides a modern alternative, which we will explore in **Tutorial 5: JavaScript Scripting**.
