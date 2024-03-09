#include "joystick_rc.h"

#include "global.h"
#include "logger.h"
JoystickRc::JoystickRc() {}

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

        if (inputs.cross_up) {
          Log::get().info("Cross");
          enabled_ = !enabled_;
        }
        if (inputs.circle_up) {
          Log::get().info("Circle");
          gbutton_event_dispatcher.dispatch("Emergency");
        }
        if (inputs.square_up) {
          Log::get().info("Square");
          gbutton_event_dispatcher.dispatch("Takeoff");
        }
        if (inputs.triangle_up) {
          Log::get().info("Triangle");
          gbutton_event_dispatcher.dispatch("Land");
        }
        if (inputs.rstick_button_up) {
          gbutton_event_dispatcher.dispatch("IP_Hover");
        }
        if (new_event && enabled_) {
          grc_event_dispatcher.dispatch(0, {inputs.lx, inputs.ly, inputs.ry, inputs.rx});
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
      }
    });
  }
}