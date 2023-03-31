#ifndef JOYSTICK_RC_H
#define JOYSTICK_RC_H

#include <thread>
#include <atomic>

#include "joystick.h"
#include "flight_control.h"
#include "image_processing.h"

class JoystickRc {
 public:
  JoystickRc(FlightControl* fc, ImageProcessing* ip);
  ~JoystickRc();
  void start();

 private:
  Joystick joystick_;
  std::thread th_;
  std::atomic<bool> run_ = {false};
  FlightControl* fc_;
  ImageProcessing* ip_;
  bool enabled_ = false;
};

#endif  // JOYSTICK_RC_H