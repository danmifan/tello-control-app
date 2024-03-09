#ifndef JOYSTICK_RC_H
#define JOYSTICK_RC_H

#include <thread>
#include <atomic>

#include "joystick.h"

class JoystickRc {
 public:
  JoystickRc();
  ~JoystickRc();
  void start();

 private:
  Joystick joystick_;
  std::thread th_;
  std::atomic<bool> run_ = {false};
  bool enabled_ = false;
};

#endif  // JOYSTICK_RC_H