#include <iostream>
#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"
#include "data.h"
#include "views/main_view.h"
#include "joystick.h"
#include "video_streaming.h"
#include "face_detection.h"

#include <signal.h>

std::atomic<bool> stop = {false};

int main(int /*argc*/, char** /*argv*/) {
  MyWindow window(1600, 900);

  FlightControl flight_control;
  DroneStatus drone_status;
  Joystick joystick;
  VideoStreaming stream(960, 720, 3);
  FaceDetection face_detection;

  MainView view(&flight_control, &stream, 960, 720);

  flight_control.start();
  drone_status.start();
  joystick.start();

  std::thread th([&]() {
    while (!stop) {
      JoystickInputs inputs;
      bool new_event = joystick.update(inputs);
      if (new_event) {
        flight_control.radioControl(inputs.lx, inputs.ly, inputs.ry, inputs.rx);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });

  // std::thread detect_th([&]() {
  //   while (1) {
  //     if (!frames_.empty()) {
  //       cv::Mat frame = frames_.front();
  //       frames_.pop_front();
  //       auto t1 = std::chrono::high_resolution_clock::now();
  //       face_detection.detect(frame);
  //       auto t2 = std::chrono::high_resolution_clock::now();
  //       int duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  //       std::cout << "Total : " << duration << std::endl;
  //     } else {
  //       std::this_thread::sleep_for(std::chrono::milliseconds(16));
  //     }
  //   }
  // });

#warning not exiting properly when closing window with an active stream

  window.init();

  window.addWidget(&view);
  view.setImage(stream.getImage());
  view.setFaceImage(face_detection.getFaceImage());
  view.setTextures(window.getTextures());
  window.update();

  window.shutdown();

  stop = true;
  th.join();

  std::cout << "Bye" << std::endl;

  return 0;
}