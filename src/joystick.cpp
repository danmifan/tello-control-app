#include "joystick.h"

#include <iostream>

Joystick::~Joystick() { close(js_); }

void Joystick::start() {
  js_ = open(device_, O_RDONLY | O_NONBLOCK);

  if (js_ == -1) perror("Could not open joystick");
}

JoystickInputs Joystick::update() {
  struct js_event event;
  while (readEvent(&event)) {
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
  JoystickInputs inputs;
  inputs.lx = 100 * axis_[LX] / MAX_VALUE;
  inputs.ly = 100 * axis_[LY] / MAX_VALUE;
  inputs.rx = 100 * axis_[RX] / MAX_VALUE;
  inputs.ry = -100 * axis_[RY] / MAX_VALUE;

  return inputs;
}

bool Joystick::readEvent(struct js_event* event) {
  ssize_t bytes;

  bytes = read(js_, event, sizeof(*event));

  return bytes == sizeof(*event);
}
