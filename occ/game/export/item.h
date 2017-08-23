#ifndef ITEM_H_
#define ITEM_H_

#include <string>

class Item
{
 public:
  using Id = int;

  static const Id invalid;

  Item(std::string name, int sprite, int sprite_count, int type, int flags)
    : name_(name),
      sprite_(sprite),
      sprite_count_(sprite_count),
      type_(type),
      flags_(flags)
  {
  }

  const std::string& get_name() const { return name_; }
  int get_sprite() const { return sprite_; }
  int get_sprite_count() const { return sprite_count_; }

  bool is_type_background() const { return type_ == 0; }
  bool is_type_middleground() const { return type_ == 1; }
  bool is_type_foreground() const { return type_ == 2; }

  // For background
  bool is_multiple_2x2() const { return is_type_background() && (flags_ & 0x01) != 0; }
  bool is_multiple_4x2() const { return is_type_background() && (flags_ & 0x02) != 0; }

  // For middleground
  bool is_solid()     const { return is_type_middleground() && (flags_ & 0x01) != 0; }
  bool is_solid_top() const { return is_type_middleground() && (flags_ & 0x02) != 0; }
  bool is_damage()    const { return is_type_middleground() && (flags_ & 0x04) != 0; }
  bool is_death()     const { return is_type_middleground() && (flags_ & 0x08) != 0; }

  // For foreground
  bool is_animated() const { return is_type_foreground() && (flags_ & 0x01) != 0; }

 private:
  std::string name_;
  int sprite_;
  int sprite_count_;
  int type_;
  int flags_;
};

#endif  // ITEM_H_
