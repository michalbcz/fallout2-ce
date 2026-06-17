# Tutorial 15: Intermediate C++ inside fallout2-ce

To confidently modify the `fallout2-ce` engine, you need to understand how the codebase handles intermediate C++ concepts. Because it is a reimplementation of 1990s C/C++ code, the patterns here differ from modern C++20 standard practices.

## 1. Pointers vs. References in the Engine

In modern C++, you are taught to use references (`&`) whenever possible, because they cannot be null.
However, in `fallout2-ce`, you will heavily encounter **raw pointers (`*`)**.

```cpp
// Common engine pattern
Object* obj = objectGet(objectId);
if (obj != nullptr) {
    obj->hp -= 10;
}
```

### Why Pointers?
1. **Nullability:** In a game engine, "not finding an object" is a common state. A pointer can be `nullptr`, making it perfect for functions like `findCritterAt(x, y)` which returns `nullptr` if the hex is empty.
2. **C-Compatibility:** Many underlying engine structures map directly to old C data, where references do not exist.

**The Rule:** Always check for `nullptr` before dereferencing an object pointer retrieved from the engine.

## 2. Memory Management

You will rarely see `std::shared_ptr` or `std::unique_ptr` in the core simulation code. The game maintains its own strict memory pools.

### Object Pools
When you spawn a critter, the engine does not just call `new Object()`. It allocates from a pre-defined memory pool.
This is because during the 90s, heap allocation (calling `malloc`/`new` during gameplay) was slow and caused fragmentation.
Instead, the engine allocates a giant block of memory for 1000 objects when the map loads, and hands out pointers to those slots.

When you modify engine code, you must use the engine's creation and destruction functions (e.g., `objectCreate`, `objectDestroy`) rather than `new` or `delete`.

## 3. Handling Input Events

Input in `fallout2-ce` is handled by intercepting SDL events and routing them into the game's legacy input buffers.

Take a look at `src/input.cc` and `src/kb.cc`.
Instead of standard event listeners, the game usually polls for input during the main game loop.

```cpp
// Conceptual input loop
while (game_is_running) {
    int key = kbGetInput();
    if (key == KEY_ESCAPE) {
        showMainMenu();
    }

    int mouseX, mouseY;
    mouseGetPosition(&mouseX, &mouseY);
    if (mouseGetButtons() & MOUSE_BUTTON_LEFT) {
        handleLeftClick(mouseX, mouseY);
    }
}
```

If you want to add a new hotkey to the engine:
1. Locate the main game loop or the specific UI window's event loop.
2. Add a check for your desired keycode (using the engine's defined `KEY_*` constants).
3. Call your custom C++ function.

## 4. Global State

In modern software architecture, global variables are considered bad practice. In the `fallout2-ce` codebase, they are everywhere.
The state of the world, the player's inventory, and current map data are often stored in global variables or static structs accessible from anywhere.
While this makes modifying state easy (you don't have to pass an `EngineContext` object down through 10 function calls), you must be extremely careful not to overwrite global state unpredictably, which will corrupt the save game.
