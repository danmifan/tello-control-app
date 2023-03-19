#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <deque>
#include <thread>
#include <linux/joystick.h>
#include <map>

#define MAX_VALUE 32767
#define DEAD_ZONE 9600

struct JoystickInputs {
  int lx = 0;
  int ly = 0;
  int rx = 0;
  int ry = 0;
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
  void start();
  bool update(JoystickInputs& inputs);
  bool readEvent(struct js_event* event);

 private:
  std::string device_ = "/dev/input/js0";
  int js_;

  std::map<JoystickAxis, int16_t> axis_;
  std::map<PSJoystickButtons, int> buttons_;
};