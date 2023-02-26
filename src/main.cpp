#include <iostream>
#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"
#include "data.h"
#include "views/main_view.h"
#include "joystick.h"
#include "video_streaming.h"
#include "face_detection.h"

int main(int /*argc*/, char** /*argv*/) {
  MyWindow window(1600, 900);

  FlightControl flight_control;
  DroneStatus drone_status;
  // Joystick joystick;
  VideoStreaming stream(960, 720, 3);
  FaceDetection face_detection;

  MainView view(&flight_control, &stream, 960, 720);

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

  std::thread th([&]() {
    while (1) {
      if (!frames_.empty()) {
        cv::Mat frame = frames_.front();
        frames_.pop_front();
        auto t1 = std::chrono::high_resolution_clock::now();
        face_detection.detect(frame);
        auto t2 = std::chrono::high_resolution_clock::now();
        int duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << "Total : " << duration << std::endl;
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
      }
    }
  });

#warning not exiting properly when closing window with an active stream

  window.init();

  window.addWidget(&view);
  view.setImage(stream.getImage());
  view.setFaceImage(face_detection.getFaceImage());
  view.setTextures(window.getTextures());
  window.update();

  window.shutdown();

  std::cout << "Bye" << std::endl;

  return 0;
}