[![C/C++ CI](https://github.com/gurka/OpenCrystalCaves/actions/workflows/cpp.yml/badge.svg)](https://github.com/gurka/OpenCrystalCaves/actions/workflows/cpp.yml)
[![#CrystalCaves on Mastodon](https://img.shields.io/badge/-%23CrystalCaves-%23303030?logo=mastodon)](https://mastodon.gamedev.place/tags/CrystalCaves)
[![Download at itch.io](https://img.shields.io/badge/-Downloads-%23303030?logo=itchdotio)](https://congusbongus.itch.io/opencrystalcaves)

# OpenCrystalCaves

OCC is an unofficial open source engine reimplementation of the game trilogy [Crystal Caves](https://en.wikipedia.org/wiki/Crystal_Caves).

Download binaries and project info at [itch.io](https://congusbongus.itch.io/opencrystalcaves)

**This repository includes the shareware version of Crystal Caves. To play the full, retail version, you can copy over the data files or purchase them at [Steam](http://store.steampowered.com/app/358260/Crystal_Caves/) or [GoG](https://www.gog.com/game/crystal_caves).**

Current state of OCC:

![Animation](/screenshots/animation.gif?raw=true "Animation")

![Screenshot 3](/screenshots/screenshot3.png?raw=true "Screenshot 3")

Currently the engine can:

- Load sprite, item and level data.
- Main level and entering levels.
- Render items that, both behind and in front of the player, as well as animated items.
- Move the player around, jumping, colliding with solid items and moving platforms.

Features that are missing:

- Loading and saving games.
- Exiting levels.
- Items.
- Enemies.
- (and much more).

## Compiling OCC

OCC is built using C++17 and requires external libraries: [SDL 2.0](https://www.libsdl.org/), [SDL_image](https://www.libsdl.org/projects/old/SDL_image/) and [SDL_mixer](https://www.libsdl.org/projects/old/SDL_mixer/) and . SDL2 must be installed and available in `/usr/include/SDL2`. Additionally the build system `cmake` must be installed.

Steps to compile (Linux, macOS):

```
git clone --recursive https://github.com/gurka/OpenCrystalCaves.git
cd OpenCrystalCaves
mkdir debug
cmake -Bdebug -DCMAKE_BUILD_TYPE=debug occ
cd debug
make
```

The binary will be available at `OpenCrystalCaves/build/occ`

Or in Windows (with [vcpkg](https://vcpkg.io)):

```
# In vcpkg dir
./vcpkg install --triplet x64-windows sdl2 --recurse
# In your projects dir
git clone --recursive https://github.com/gurka/OpenCrystalCaves.git
cd OpenCrystalCaves
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake occ -A x64
```

The Visual Studio project will be available at `OpenCrystalCaves/build`

## Running OCC

OCC requires data files from the original Crystal Caves (any episode). Either install it via Steam or GoG, or copy the game data to the `media` folder in the occ package (such as `CC1.GFX`).
