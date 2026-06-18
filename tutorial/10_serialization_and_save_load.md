# Tutorial 10: Serialization and the Save/Load System

One of the most complex parts of any large RPG is the Save/Load system. In `fallout2-ce`, the game needs to remember the state of every dropped item, every dead critter, the player's stats, and the progress of hundreds of quests.

This tutorial explores how the engine accomplishes this, which is a fantastic case study in **C++ binary serialization**.

## 1. What is Serialization?

Serialization is the process of translating a data structure or object state into a format that can be stored (for example, in a file) and reconstructed later.

In modern applications, you might serialize data to JSON or XML. However, `fallout2-ce` (mirroring the original 1998 engine) serializes directly to **binary**. This is incredibly fast and produces small files, but it requires strict memory management.

## 2. Plain Old Data (POD) Structs

To save an object to a binary file easily, the object must be a POD (Plain Old Data) type. This means it contains only basic data types (ints, floats, arrays) and no complex C++ objects (like `std::string` or `std::vector`), and has no virtual functions.

Because of this, many core game structures in `fallout2-ce` are simple structs:

```cpp
struct CritterState {
    int hp;
    int maxHp;
    int actionPoints;
    int flags;
    // Note: No std::vector or pointers here!
};
```

## 3. Writing to Disk

When the player hits "Save", the engine traverses the game world and writes these structs directly to the save file.

The `loadsave.cc` and related files manage this. Instead of parsing text, the engine writes raw bytes:

```cpp
// A simplified conceptual example of saving a struct
CritterState playerState = getPlayerState();
fileWrite(&playerState, sizeof(CritterState), 1, saveFileHandle);
```
Here, `fileWrite` (part of the engine's VFS) takes the memory address of the struct (`&playerState`) and writes exactly `sizeof(CritterState)` bytes to the file.

## 4. Reading from Disk (Deserialization)

Loading the game is the reverse process. The engine allocates memory for the struct and reads the bytes straight from the disk back into memory.

```cpp
CritterState loadedState;
fileRead(&loadedState, sizeof(CritterState), 1, saveFileHandle);
```

## 5. The Danger of Binary Serialization

While fast, binary serialization is very brittle.

**What happens if you modify the engine and add a new variable to `CritterState`?**
If you add `int karma;` to the middle of `CritterState`, the size of the struct changes.
If a user tries to load an old save file with the new engine, the engine will read the wrong number of bytes, the data will be offset, and the game will crash or behave unpredictably.

## 6. How fallout2-ce Handles Versioning

To prevent save file corruption, engines often write a **version number** at the very beginning of the save file.

When loading, the engine reads the version number first:
- If `version == 1`, read the data using the old struct layout.
- If `version == 2`, read the data using the new struct layout.

Because `fallout2-ce` is striving for bug-for-bug compatibility with original Fallout 2 save files, developers must be extremely careful *never* to change the size or layout of structs that map to the original game's save format.

## 7. Summary

The save/load system in `fallout2-ce` is a perfect example of legacy C/C++ memory management:
- It relies on raw binary memory copies (`memcpy`, `fileRead`, `fileWrite`).
- It requires structs to be tightly packed and strictly sized.
- It prioritizes speed and file size over human-readability.

In the next tutorial, we will explore another engine-level trick: **8-bit color palettes and animation**.
