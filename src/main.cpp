#include <iostream>
#include <signal.h>

#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"
#include "data.h"
#include "views/main_view.h"
#include "joystick.h"
#include "video_streaming.h"

#include <opencv2/core.hpp>

int main(int argc, char **argv) {
  MyWindow window(800, 600);

  FlightControl flight_control;
  // DroneStatus drone_status;
  // Joystick joystick;
  VideoStreaming stream;

  // std::cout << "OpenCV version : " << CV_VERSION << std::endl;
  // std::cout << "Major version : " << CV_MAJOR_VERSION << std::endl;
  // std::cout << "Minor version : " << CV_MINOR_VERSION << std::endl;
  // std::cout << "Subminor version : " << CV_SUBMINOR_VERSION << std::endl;

  MainView view(&flight_control);

  flight_control.init();
  stream.init();
  // drone_status.init();
  // joystick.init();

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

  window.init();
  window.addWidget(&view);
  window.update();

  window.shutdown();

  std::cout << "Bye" << std::endl;

  return 0;
}