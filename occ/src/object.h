// TODO: Erase? Same as class Item?
#if 0
#ifndef OBJECT_H_
#define OBJECT_H_

#include "vector.h"

class Object
{
 public:
  Object(const Vector<unsigned>& size, unsigned spriteId)
    : size_(size),
      spriteId_(spriteId)
  {
  }

  const Vector<int>& getSize() const { return size_; }
  unsigned getSpriteId() const { return spriteId_; }

 private:
  Vector<int> size_;
  unsigned spriteId_;
};

#endif  // OBJECT_H_
#endif
