#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <map>

#define MAX_VALUE 32767
#define DEAD_ZONE 9600

struct JoystickInputs {
  int lx = 0;
  int ly = 0;
  int rx = 0;
  int ry = 0;
  bool cross_pressed = false;
  bool circle_pressed = false;
  bool square_pressed = false;
  bool triangle_pressed = false;
  bool rstick_button_pressed = false;
  bool cross_up = false;
  bool square_up = false;
  bool triangle_up = false;
  bool circle_up = false;
  bool rstick_button_up = false;
};

enum JoystickAxis { LX = 0, LY = 1, RX = 3, RY = 4, LT = 2, RT = 5 };

enum JoystickButtons {
  A = 0,
  B = 1,
  Y = 2,
  X = 3,
  LB = 4,
  RB = 5,
  VIEW = 6,
  MENU = 7,
  XBOX = 8,
  LEFT_CLICK = 9,
  RIGHT_CLICK = 10
};

enum PSJoystickButtons {
  CROSS = 0,
  CIRCLE = 1,
  TRIANGLE = 2,
  SQUARE = 3,
  L1 = 4,
  R1 = 5,
  L2 = 6,
  R2 = 7,
  SHARE = 8,
  OPTIONS = 9,
  HOME = 10,
  L3 = 11,
  R3 = 12
};

class Joystick {
 public:
  ~Joystick();
  bool init();
  bool update(JoystickInputs& inputs);
  bool readEvent(struct js_event* event);

 private:
  std::string device_ = "/dev/input/js0";
  int js_;

  std::map<JoystickAxis, int16_t> axis_;
  std::map<PSJoystickButtons, int> buttons_;
  JoystickInputs previous_inputs_;
};

#endif  // JOYSTICK_H