#include "joystick_rc.h"

#include "logger.h"
JoystickRc::JoystickRc(FlightControl* fc, ImageProcessing* ip) : fc_(fc), ip_(ip) {}

JoystickRc::~JoystickRc() {
  if (run_) {
    run_ = false;
    th_.join();
  }
}

void JoystickRc::start() {
  if (joystick_.init()) {
    run_ = true;
    th_ = std::thread([&]() {
      while (run_) {
        JoystickInputs inputs;
        bool new_event = joystick_.update(inputs);
        if (inputs.square_up) {
          enabled_ = true;
        }
        if (inputs.triangle_up) {
          enabled_ = false;
        }
        if (inputs.square_up) {
          // Log::get().info("Square");
          fc_->takeoff();
        }
        if (inputs.triangle_up) {
          fc_->land();
          // Log::get().info("Triangle");
        }
        if (inputs.rstick_button_up) {
          ip_->setHoverPose();
        }
        if (new_event && enabled_) {
          fc_->radioControl(inputs.lx, inputs.ly, inputs.ry, inputs.rx);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    });
  }
}