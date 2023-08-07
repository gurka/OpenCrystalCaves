[![C/C++ CI](https://github.com/gurka/OpenCrystalCaves/actions/workflows/cpp.yml/badge.svg)](https://github.com/gurka/OpenCrystalCaves/actions/workflows/cpp.yml)

# OpenCrystalCaves
OCC is an unofficial open source engine reimplementation of the game trilogy [Crystal Caves](https://en.wikipedia.org/wiki/Crystal_Caves).

**Note that the sprites are not available in this repository. You need a copy of the original Crystal Caves, shareware or retail, to be able to extract the sprites and use this engine, see below. The shareware Crystal Caves can be found online; the full, retail version can be purchased at [Steam](http://store.steampowered.com/app/358260/Crystal_Caves/) or [GoG](https://www.gog.com/game/crystal_caves).**

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

OCC is built using C++14 and requires three external libraries: [SDL 2.0](https://www.libsdl.org/), [SDL 2.0 TTF](https://www.libsdl.org/projects/SDL_ttf/) and [nlohmann's JSON](https://nlohmann.github.io/json/). The SDL libraries must be installed and available in `/usr/include/SDL2` and the JSON library is included in this repository as a git submodule. Additionally the build system `cmake` must be installed.

Steps to compile:
```
git clone --recursive https://github.com/gurka/OpenCrystalCaves.git
cd OpenCrystalCaves
mkdir debug
cd debug
cmake ../occ -DCMAKE_BUILD_TYPE=debug
make
```

The binary will be available at `OpenCrystalCaves/debug/occ/occ`

## Running OCC
The file `media/sprites.bmp` is required and not available in this repository. It can be generated from the original Crystal Caves (any episode) game files using the Python script `tools/sprite-extractor.py`. The script takes a path to the `CC1.GFX` file and outputs a BMP file:

```
python tools/sprite-extractor.py CC1/CC1.GFX
```
