#pragma once

#include <memory>
#include <string>
#include <vector>

#include "exe_data.h"
#include "object_manager.h"

enum class LevelId;
struct Level;
class Item;

namespace LevelLoader
{

std::unique_ptr<Level> load_level(LevelId level_id, const ObjectManager& object_manager);
std::unique_ptr<Level> load(ExeData& exe_data, const LevelId level_id, const ObjectManager& object_manager);

}
