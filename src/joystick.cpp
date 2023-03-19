#include "joystick.h"

#include "logger.h"
#include <iostream>

Joystick::~Joystick() { close(js_); }

int clip(int n, int lower, int upper) { return std::max(lower, std::min(n, upper)); }

void Joystick::start() {
  js_ = open(device_.c_str(), O_RDONLY | O_NONBLOCK);

  if (js_ == -1) {
    Log::get().error("Could not open joystick device : " + device_);
  } else {
    Log::get().info(device_ + " successfully opened");
  }
}

bool Joystick::update(JoystickInputs& inputs) {
  struct js_event event;
  bool new_event = false;
  while (readEvent(&event)) {
    new_event = true;

    switch (event.type) {
      case JS_EVENT_AXIS:

        if (-DEAD_ZONE < event.value && event.value < DEAD_ZONE) {
          axis_[(JoystickAxis)event.number] = 0;
        } else {
          axis_[(JoystickAxis)event.number] = event.value;
        }

        break;

      case JS_EVENT_BUTTON:
        buttons_[(PSJoystickButtons)event.number] = event.value;
        break;

      default:
        break;
    }
  }

  inputs.lx = 100 * axis_[LX] / MAX_VALUE;
  inputs.ly = -100 * axis_[LY] / MAX_VALUE;
  inputs.rx = 100 * axis_[RX] / MAX_VALUE;
  inputs.ry = -100 * axis_[RY] / MAX_VALUE;

  inputs.lx = clip(inputs.lx, -100, 100);
  inputs.ly = clip(inputs.ly, -100, 100);
  inputs.rx = clip(inputs.rx, -100, 100);
  inputs.ry = clip(inputs.ry, -100, 100);

  return new_event;
}

bool Joystick::readEvent(struct js_event* event) {
  ssize_t bytes;

  bytes = read(js_, event, sizeof(*event));

  return bytes == sizeof(*event);
}
