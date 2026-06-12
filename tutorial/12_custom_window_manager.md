# Tutorial 12: The Custom Window Manager

If you have used modern engines like Unity, Unreal, or Godot, you are used to robust UI systems with Canvases, Anchors, and specialized UI components.
`fallout2-ce` is different. It implements its own complete UI Windowing system entirely from scratch in C++.

If you want to modify a menu, add a button, or change how text is displayed, you need to understand `src/window.cc` and `src/window_manager.cc`.

## 1. The Single Operating System Window

When you run `fallout2-ce`, the operating system (Windows/macOS/Linux) provides exactly **one window** (via SDL2).
Everything inside that window—the game world, the Pip-Boy, the inventory screen, the dialogue boxes—is manually drawn by the game's internal code.

## 2. Internal Windows

The engine maintains a stack of its own internal "Windows".

When the game needs to show the character inventory:
1. It calls a function like `windowCreate(x, y, width, height, color)`.
2. This creates an internal representation of a rectangular area.
3. The engine pushes this new window onto a stack.

Because it's a stack, the top-most window intercepts mouse clicks first. This is how the engine handles modal dialogs (like "Are you sure you want to quit?"). If the quit confirmation is on top of the stack, clicks won't register on the buttons beneath it.

## 3. Drawing and Clipping

When a window is created, the engine allocates a buffer of memory exactly the size of the window (in 8-bit pixels, as we learned in Tutorial 11).

When you draw text or an image *to a window*, the engine uses **clipping**.
Clipping ensures that if you try to draw a long string of text near the edge of an internal window, the text doesn't bleed out into the rest of the screen. The custom drawing routines in `fallout2-ce` calculate the boundaries and only modify pixels inside the window's memory buffer.

Once all the internal windows have been drawn into their respective buffers, the window manager composites them together, from bottom to top, to create the final frame that is sent to SDL.

## 4. UI Elements (Buttons and Regions)

The engine doesn't have a complex object-oriented `Button` class hierarchy. Instead, it relies heavily on "Regions".

A region is simply an invisible rectangle defined within a window.
You register a region with coordinates:
```cpp
// Conceptual registration of a UI button region
int buttonId = windowAddRegion(myWindow, x, y, width, height);
```

You then assign callback functions to that region for different mouse states:
- `onMouseEnter`
- `onMouseLeave`
- `onMouseDown`
- `onMouseUp`

When the player moves the mouse, the engine checks the cursor's coordinates against the regions in the top-most window. If it finds a match, it triggers the callback.

This means that to draw a button, you usually:
1. Draw the button graphic onto the window's buffer.
2. Define an invisible region over the exact same coordinates to capture the clicks.

## 5. Summary

Modifying the UI in `fallout2-ce` is a highly manual process. You do not drag and drop components; you calculate X and Y coordinates, draw raw pixel data to buffers, and define invisible bounding boxes to catch user input.

This concludes our tutorial series. You should now have a solid grasp of how `fallout2-ce` leverages C++, handles memory and data, renders its graphics, and processes game logic! Happy modding and coding!
