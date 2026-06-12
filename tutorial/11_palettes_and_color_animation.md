# Tutorial 11: Palettes and Color Animation

Modern games render in True Color (32-bit: Red, Green, Blue, Alpha). You can specify exactly what color every pixel should be.
Fallout 2, however, is a game from 1998 designed to run on systems with highly limited memory. It uses an **8-bit Indexed Color system**.

Understanding this system is crucial for working on the graphics engine in `fallout2-ce` (primarily `src/palette.cc` and `src/draw.cc`).

## 1. What is an 8-bit Palette?

In an 8-bit system, an image doesn't store the actual colors (RGB) of its pixels. Instead, each pixel stores a single number from `0` to `255`.

This number is an **index**. The game holds a master "Palette" in memory—an array of 256 colors.
When the engine renders a pixel with value `5`, it looks at slot `5` in the palette, finds the actual RGB color stored there, and draws that color to the screen.

```cpp
// Conceptual Palette lookup
Color screenPixelColor = globalPalette[pixelValue];
```

## 2. The Power of Palette Animation

Why use this restrictive system today? Because it allows for incredibly cheap and powerful visual effects through a technique called **Palette Animation**.

Imagine a toxic barrel filled with glowing green goo. In a modern game, you would need multiple animation frames of the goo bubbling, which takes up memory.

In Fallout 2:
1. The artist paints the goo using specific palette indices (e.g., indices 229 through 232).
2. Instead of animating the *image*, the engine **animates the palette**.
3. The engine shifts the RGB values assigned to indices 229-232 every few milliseconds.
4. Suddenly, the static image appears to glow and animate, because the colors the pixels are referencing are changing!

This is how `fallout2-ce` achieves blinking computer screens, glowing toxic waste, and flowing water with almost zero CPU overhead. Look at `paletteCycle` in the engine source to see this in action.

## 3. The Day/Night Cycle

Another massive benefit of palettes is lighting.
Fallout 2 has a day/night cycle. Doing complex lighting calculations on every pixel of an isometric map would have melted a 1998 CPU.

Instead, when the sun goes down, the engine simply calculates a new master palette where all the RGB values are darkened, shifted slightly towards blue, and applies it. Instantly, the entire game world becomes night without a single change to the map data.

## 4. Modernizing Palettes in CE

While `fallout2-ce` recreates this exact 8-bit logic to remain perfectly faithful to the original art assets (`.frm` files), it eventually has to display the result on your modern 32-bit monitor.

Behind the scenes, SDL2 requires a 32-bit texture to draw to the window.
So, `fallout2-ce` creates an 8-bit software surface where the game logic happens, and then translates those 8-bit pixels through the active palette into a modern 32-bit SDL Texture just before throwing it to the graphics card (see `src/svga.cc` and `src/window.cc`).

## 5. Summary

Indexed color is a brilliant historical optimization. By separating the *pixel data* from the *color data*:
- Images (`.frm` files) are extremely small (1 byte per pixel).
- Complex animations and global lighting changes are achieved instantly by modifying an array of 256 colors.

In our final architectural tutorial, we'll look at how the game handles UI: **Tutorial 12: The Custom Window Manager**.
