#include "joystick_rc.h"

#include "event.h"
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
          EnableButtonEvent event(enabled_);
          gevent_dispatcher.dispatch("JoystickEnabled", event);
        }
        if (inputs.circle_up) {
          Log::get().info("Circle");
          gevent_dispatcher.dispatch("Emergency", {});
        }
        if (inputs.square_up) {
          Log::get().info("Square");
          gevent_dispatcher.dispatch("Takeoff", {});
        }
        if (inputs.triangle_up) {
          Log::get().info("Triangle");
          gevent_dispatcher.dispatch("Land", {});
        }
        if (inputs.rstick_button_up) {
          gevent_dispatcher.dispatch("IP_Hover", {});
        }
        if (new_event && enabled_) {
          RCEvent rc_event{inputs.lx, inputs.ly, inputs.ry, inputs.rx};
          gevent_dispatcher.dispatch("RCCommands", rc_event);
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
      }
    });
  }
}