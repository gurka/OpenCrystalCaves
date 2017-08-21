#ifndef CONFIG_H_
#define CONFIG_H_

#include <cassert>

// "default" includes
#include "geometry.h"
#include "math.h"

namespace config
{

// Static
constexpr int CAMERA_WIDTH  = 320;
constexpr int CAMERA_HEIGHT = 192;

// Should be dynamic in the future (and not fixed ratio)
constexpr int SCREEN_SCALE = 3;

constexpr int MS_PER_UPDATE = 50;

}

#endif  // CONFIG_H_
