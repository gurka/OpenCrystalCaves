#ifndef ITEM_H_
#define ITEM_H_

class Item
{
 public:
  Item()
    : valid_(false),
      sprite_(-1),
      type_(0),
      amount_(0)
  {
  }

  Item(int sprite, int type, int amount)
    : valid_(true),
      sprite_(sprite),
      type_(type),
      amount_(amount)
  {
  }

  bool valid() const { return valid_; }

  int get_sprite() const { return sprite_; }
  int get_type() const { return type_; }
  int get_amount() const { return amount_; }

  static const Item INVALID;

 private:
  bool valid_;
  int sprite_;
  int type_;
  int amount_;
};

#endif  // ITEM_H_
