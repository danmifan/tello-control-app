#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"

#include "views/main_view.h"
#include "views/flight_controls_view.h"
#include "views/drone_status_view.h"

#include "joystick_rc.h"
#include "video_streaming.h"
#include "face_detection.h"
#include "image_processing.h"
#include "logger.h"

int main(int /*argc*/, char** /*argv*/) {
  MyWindow window(1600, 900);

  FlightControl flight_control;
  DroneStatus drone_status;
  VideoStreaming stream(960, 720, 3);
  FaceDetection face_detection;
  ImageProcessing processing(&flight_control);
  JoystickRc joystick_rc(&flight_control, &processing);

  // Views
  MainView main_view(960, 720);
  FlightControlView fc_view(&flight_control, &stream);
  DroneStatusView drone_status_view;

  flight_control.start();
  drone_status.start();
  processing.start();
  joystick_rc.start();

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

  window.init();

  window.addView(&main_view);
  window.addView(&fc_view);
  window.addView(&drone_status_view);

  main_view.setImage(stream.getImage());
  main_view.setFaceImage(face_detection.getFaceImage());
  main_view.setImgProcImage(processing.getImage());
  main_view.setTextures(window.getTextures());
  // processing.setImage(stream.getImage());
  processing.setEvent(main_view.getEvent());
  window.update();

  window.shutdown();

  std::cout << "Bye" << std::endl;

  return 0;
}