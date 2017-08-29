#ifndef INPUT_H_
#define INPUT_H_

struct Input
{
  struct Button
  {
    bool pressed  = false;
    bool repeated = false;
  };

  Button up    = Button();
  Button down  = Button();
  Button left  = Button();
  Button right = Button();
  Button lctrl = Button();
  Button lalt  = Button();

  Button num_1 = Button();
  Button num_2 = Button();
  Button num_3 = Button();
  Button num_4 = Button();

  bool quit    = false;
};

#endif  // INPUT_H_
