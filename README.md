[![Build Status](https://travis-ci.org/gurka/OpenCrystalCaves.svg?branch=master)](https://travis-ci.org/gurka/OpenCrystalCaves)

# OpenCrystalCaves
OCC is an unofficial open source engine reimplementation of the game trilogy [Crystal Caves](https://en.wikipedia.org/wiki/Crystal_Caves).

**Note that the sprites are not available in this repository. You need to own the original Crystal Caves to be able to extract the sprites and use this engine, see below. The original Crystal Caves can be purchased at [Steam](http://store.steampowered.com/app/358260/Crystal_Caves/).**

Current state of OCC:

![Screenshot 1](/screenshots/screenshot1.png?raw=true "Screenshot 1")

![Screenshot 2](/screenshots/screenshot2.png?raw=true "Screenshot 2")

![Screenshot 3](/screenshots/screenshot3.png?raw=true "Screenshot 3")

Currently the engine can:

- Load sprite, item and level data.
- Render a level that is selected at compile time.
- Render items that, both behind and infront of the player, as well as animated items.
- Move the player around, jumping, colliding with solid items and moving platforms.

Features that are missing:

- Main menu including starting a new game and loading a saved game.
- Entering levels.
- Shooting.
- Enemies.
- (and much more).

## Compiling OCC

OCC is built using C++11 and requires three external libraries: [SDL 2.0](https://www.libsdl.org/), [SDL 2.0 TTF](https://www.libsdl.org/projects/SDL_ttf/) and [nlohmann's JSON](https://nlohmann.github.io/json/). The SDL libraries must be installed and available in `/usr/include/SDL2` and the JSON library is included in this repository as a git submodule. Additionally the build system `cmake` must be installed.

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
Two files are required and not available in this repository: `DejaVuSansMono.ttf` and `sprites.bmp`.

`DejaVuSansMono.ttf` can be downloaded for free [here](https://dejavu-fonts.github.io/), or might already be available in your Linux installation. In Debian 9 the font is available at `/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf`

`sprites.bmp` can be generated from the original Crystal Caves (episode 1) game files using the Python script `utils/sprite-extractor.py`. The script takes a path to the `CC1.GFX` file and outputs a PNG file that then should be converted to BMP (using e.g. ImageMagick):

```
python utils/sprite-extractor.py CC1/CC1.GFX
convert sprites.png sprites.bmp
```

With these two files inside the `media/` directory it's just matter of running the executable.
