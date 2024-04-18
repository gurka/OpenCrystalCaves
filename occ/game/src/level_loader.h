#pragma once

#include <string>
#include <memory>
#include <vector>

#include "exe_data.h"

enum class LevelId;
struct Level;
class Item;

namespace LevelLoader
{

std::unique_ptr<Level> load_level(LevelId level_id);
std::unique_ptr<Level> load(ExeData& exe_data, const LevelId level_id);

}
