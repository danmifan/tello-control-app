#include "joystick.h"

#include <iostream>

Joystick::~Joystick() { close(js_); }

void Joystick::start() {
  js_ = open(device_, O_RDONLY | O_NONBLOCK);

  if (js_ == -1) perror("Could not open joystick");

  th_ = std::thread([&]() {
    while (1) {
      int left_joystick_pressed = 0;
      int right_joystick_pressed = 0;

      if (read_event(js_, &event_) == 0) {
        if (event_.type == JS_EVENT_AXIS) {
          const size_t current_axis = get_axis_state(&event_, axes_);

          if (current_axis == 0) left_joystick_pressed = 1;
          if (current_axis == 1) right_joystick_pressed = 1;

          if (-dead_zone_ < axes_[current_axis].x && axes_[current_axis].x < dead_zone_) {
            axes_[current_axis].x = 0;
            left_joystick_pressed = 0;
          }

          if (-dead_zone_ < axes_[current_axis].y && axes_[current_axis].y < dead_zone_) {
            axes_[current_axis].y = 0;
            right_joystick_pressed = 0;
          }
        }
      }

      JoystickData last_joystick_data;
      last_joystick_data.left_joystick_x = 100 * axes_[0].x / max_axis_value_;
      last_joystick_data.left_joystick_y = -100 * axes_[0].y / max_axis_value_;
      last_joystick_data.right_joystick_x = 100 * axes_[1].x / max_axis_value_;
      last_joystick_data.right_joystick_y = -100.0f * axes_[1].y / max_axis_value_;

      data_.push_back(last_joystick_data);
    }
  });
}

bool Joystick::getData(JoystickData& data) {
  if (!data_.empty()) {
    data = data_.back();
    return true;
  }
  return false;
}