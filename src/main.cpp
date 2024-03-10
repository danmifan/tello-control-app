// ORBSLAM
//  #include <System.h>

#include "event.h"

#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"

#include "views/main_view.h"
#include "views/flight_controls_view.h"
#include "views/drone_status_view.h"

#include "joystick_rc.h"
#include "video_streaming.h"
#include "image_processing.h"
#include "logger.h"

int main(int /*argc*/, char** /*argv*/) {
#warning Add width/height to json config file
  MyWindow window(1600, 900, 60, "Tello control");

  FlightControl flight_control;
  DroneStatus drone_status;
  VideoStreaming stream(960, 720, 3);
  ImageProcessing processing(960, 720, 3);
  JoystickRc joystick_rc;

  // Views
  MainView main_view(960, 720);
  FlightControlView fc_view;
  DroneStatusView drone_status_view;

  flight_control.start();
  drone_status.start();
  processing.start();
  joystick_rc.start();

  gevent_dispatcher.appendListener("Streamon", [&](const Event&) { stream.start(); });
  gevent_dispatcher.appendListener("Streamoff", [&](const Event&) { stream.stop(); });

  // ORB_SLAM3::System slam("/home/vincent/workspace/ORB_SLAM3/Vocabulary/ORBvoc.txt", "",
  // ORB_SLAM3::System::MONOCULAR, false); float image_scale = slam.GetImageScale();

  window.init();

  window.addView(&main_view);
  window.addView(&fc_view);
  window.addView(&drone_status_view);

  main_view.setImage(stream.getImage());
  main_view.setImgProcImage(processing.getImage());
  main_view.setTextures(window.getTextures());
  // processing.setImage(stream.getImage());

  window.update();

  window.shutdown();

  std::cout << "Bye !" << std::endl;

  return 0;
}