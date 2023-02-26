#include <iostream>
#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"
#include "data.h"
#include "views/main_view.h"
#include "joystick.h"
#include "video_streaming.h"

int main(int /*argc*/, char** /*argv*/) {
  MyWindow window(1600, 900);

  FlightControl flight_control;
  DroneStatus drone_status;
  // Joystick joystick;
  VideoStreaming stream;

  MainView view(&flight_control, &stream);

  flight_control.start();
  drone_status.start();
  // joystick.start();

  // std::thread th([&]() {
  //   while (1) {
  //     JoystickData data;
  //     if (joystick.getData(data)) {
  //       if (flight_control.isFlying()) {
  //         flight_control.radioControl(data.left_joystick_x, data.left_joystick_y,
  //                                     data.right_joystick_y, data.right_joystick_x);
  //         std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //       }
  //     }
  //   }
  // });

#warning not exiting properly when closing window with an active stream

  window.init();

  stream.setImage(window.getImage());

  window.addWidget(&view);
  window.update();

  window.shutdown();

  std::cout << "Bye" << std::endl;

  return 0;
}