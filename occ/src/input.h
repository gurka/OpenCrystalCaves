#ifndef INPUT_H_
#define INPUT_H_

struct Input
{
  struct Button
  {
    bool pressed = false;
    bool repeated = false;
  };

  Button up;
  Button down;
  Button left;
  Button right;
  Button space;

  Button num_1;
  Button num_2;
  Button num_3;
  Button num_4;

  bool quit  = false;
};

#endif  // INPUT_H_
