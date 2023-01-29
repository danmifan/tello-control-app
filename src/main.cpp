#include <iostream>
#include <signal.h>

#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"
#include "data.h"
#include "views/main_view.h"
#include "joystick.h"

int main(int argc, char **argv) {
  MyWindow window(800, 600);

  FlightControl flight_control;
  DroneStatus drone_status;
  Joystick joystick;

  MainView view(&flight_control);

  flight_control.init();
  drone_status.init();
  joystick.init();

  std::thread th([&]() {
    while (1) {
      JoystickData data;
      if (joystick.getData(data)) {
        // flight_control.radioControl(data.left_joystick_x, data.left_joystick_y,
        //                             data.right_joystick_y, data.right_joystick_x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
  });

  window.init();
  window.addWidget(&view);
  window.update();

  window.shutdown();

  std::cout << "Bye" << std::endl;

  return 0;
}