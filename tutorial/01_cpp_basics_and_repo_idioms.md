# Tutorial 1: C++ Basics & Fallout2-CE Idioms

Welcome to the Fallout 2 Community Edition (fallout2-ce) project! This tutorial is designed for junior developers to get acquainted with the essential C++ concepts and specific coding idioms used in this repository.

## 1. C++ Basics: The Must-Knows

Because `fallout2-ce` is a reimplementation of a classic 1998 game, its architecture heavily bridges older C paradigms with C++. Here are the absolute basics you need to master.

### Pointers and Memory Management
In modern C++, we often rely on smart pointers (`std::unique_ptr`, `std::shared_ptr`). However, in a legacy engine reimplementation, you will frequently encounter **raw pointers**.
- **Pointers (`*`)** hold memory addresses.
- You must be careful about memory leaks. When allocating memory manually with `new` or `malloc`, ensure it is freed with `delete` or `free`.
- **References (`&`)** act as aliases to existing variables and are safer than pointers because they cannot be null.

### Structs and Classes
In C++, `struct` and `class` are almost identical, except `struct` defaults to `public` visibility, while `class` defaults to `private`.
In `fallout2-ce`, you'll see a lot of plain-old-data (POD) structs used to represent in-game objects, map data, and state, closely matching the original game's memory layouts.

### Macros and Preprocessor Directives
You will see `#define`, `#ifdef`, and `#ifndef` frequently. These are evaluated by the preprocessor before compilation. They are heavily used here for platform-specific code (Windows vs Linux vs macOS) and debugging (e.g., `#ifndef NDEBUG`).

---

## 2. Repo Idioms: How fallout2-ce is Written

To effectively read and contribute to `fallout2-ce`, you need to understand its unique structure.

### The `fallout` Namespace
Almost all game logic and functions are enclosed within the `fallout` namespace. This prevents name collisions with standard libraries or third-party dependencies.
```cpp
namespace fallout {
    void doSomethingEngineRelated();
}
```

### C-Style C++ (Reverse Engineered Architecture)
Because the engine is reverse-engineered from the original Fallout 2 binary, much of the C++ code looks like C. You will see:
- Extensive use of global variables for game state.
- Standalone functions rather than heavily object-oriented classes.
- Fixed-size arrays and older memory manipulation techniques (`memcpy`, `memset`).

### Virtual File System (VFS): `src/db.h`
**This is critical.** Do not use `std::ifstream`, `fopen`, or standard file I/O to read game assets!
Fallout 2 stores its assets in large archive files (`.dat` files). To read a script, image, or map, the engine uses a Virtual File System defined in `src/db.h`.
- Use `fileOpen(path, mode)` to open a file.
- Use `fileRead(buffer, size, iterations, fileHandle)`.
- Use `fileGetSize(fileHandle)` and `fileSeek(fileHandle, offset, origin)`.
- Always close files with `fileClose(fileHandle)`.

### SDL Integration and `SDL_RWops`
`fallout2-ce` uses SDL2 for windowing, input, and audio (via SDL_mixer).
Because SDL normally expects standard files, but we keep files in `.dat` archives, we must bridge SDL with our VFS.
When passing a file from the VFS to an SDL function (like loading an image or sound), we wrap our VFS file handle in an `SDL_RWops` structure. This allows SDL to stream directly out of the game's `.dat` files without memory leaks.

### Debugging
Debugging native C++ code in this project involves the `NDEBUG` macro in `src/debug.cc`.
The engine relies on `SDL_LogMessageV` for logging.
Additionally, the game has internal debug settings controlled via the `fallout2.cfg` file (e.g., `[debug]` section `mode`, `show_script_messages`).

### Next Steps
Now that you understand the C++ foundations and repo-specific quirks, move on to **Tutorial 2: Fallout2-CE Architecture** to understand the game loop and structural flow.
