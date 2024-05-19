[![C/C++ CI](https://github.com/gurka/OpenCrystalCaves/actions/workflows/cpp.yml/badge.svg)](https://github.com/gurka/OpenCrystalCaves/actions/workflows/cpp.yml)
[![#CrystalCaves on Mastodon](https://img.shields.io/badge/-%23CrystalCaves-%23303030?logo=mastodon)](https://mastodon.gamedev.place/tags/CrystalCaves)

# OpenCrystalCaves

OCC is an unofficial open source engine reimplementation of the game trilogy [Crystal Caves](https://en.wikipedia.org/wiki/Crystal_Caves).

**This repository includes the shareware version of Crystal Caves. To play the full, retail version, you can copy over the data files or purchase them at [Steam](http://store.steampowered.com/app/358260/Crystal_Caves/) or [GoG](https://www.gog.com/game/crystal_caves).**

Current state of OCC:

![Screenshot 1](/screenshots/screenshot1.png?raw=true "Screenshot 1")

![Screenshot 2](/screenshots/screenshot2.png?raw=true "Screenshot 2")

![Screenshot 3](/screenshots/screenshot3.png?raw=true "Screenshot 3")

Currently the engine can:

- Load sprite, item and level data.
- Render a level that is selected at compile time.
- Render items that, both behind and in front of the player, as well as animated items.
- Move the player around, jumping, colliding with solid items and moving platforms.

Features that are missing:

- Main menu including starting a new game and loading a saved game.
- Entering levels.
- Shooting.
- Enemies.
- (and much more).

## Compiling OCC

OCC is built using C++17 and requires external libraries: [SDL 2.0](https://www.libsdl.org/) and [SDL_image](https://www.libsdl.org/projects/old/SDL_image/). SDL2 must be installed and available in `/usr/include/SDL2`. Additionally the build system `cmake` must be installed.

Steps to compile (Linux, macOS):

```
git clone --recursive https://github.com/gurka/OpenCrystalCaves.git
cd OpenCrystalCaves
mkdir debug
cd debug
cmake ../occ -DCMAKE_BUILD_TYPE=debug
make
```

The binary will be available at `OpenCrystalCaves/debug/occ/occ`

Or in Windows (with [vcpkg](https://vcpkg.io)):

```
# In vcpkg dir
./vcpkg install --triplet x64-windows sdl2 --recurse
# In your projects dir
git clone --recursive https://github.com/gurka/OpenCrystalCaves.git
cd OpenCrystalCaves
mkdir debug
cd debug
cmake -B debug -DCMAKE_BUILD_TYPE=debug -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ../occ -A x64
```

The Visual Studio project will be available at `OpenCrystalCaves/debug`

## Running OCC

OCC requires data files from the original Crystal Caves (any episode). Either install it via Steam or GoG, or copy the game data to the same folder as OCC (such as `CC1.GFX`).
