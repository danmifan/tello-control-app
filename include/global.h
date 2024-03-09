#ifndef GLOBAL_H
#define GLOBAL_H

#include <deque>
#include <map>
#include "drone_status.h"
#include <opencv2/core.hpp>

#include <eventpp/eventdispatcher.h>

#warning Add to json config file
#define VIDEO_WIDTH 960
#define VIDEO_HEIGHT 720

struct ArucoMarker {
  int id;
  cv::Vec3f euler;
  cv::Vec3d tvec;
};

struct ArucoDetectorStatus {
  std::vector<ArucoMarker> markers;
};

enum EventType {
  DEMO = 0,
};

struct AEvent {
  int type;
};

struct DemoEvent : AEvent {
  int a;
  int b;

  DemoEvent() {
    type = EventType::DEMO;
  }
};

// struct ButtonEventPolicy {
//   static int getEvent(const ButtonEvent& e) {
//     // return e.type;
//   }
// };

// extern eventpp::EventDispatcher<DemoEvent, void()> gevent_dispatcher;
// extern eventpp::EventDispatcher<std::string, void(const ButtonEvent&),ButtonEventPolicy> gevent_dispatcher;
extern eventpp::EventDispatcher<std::string, void()> gbutton_event_dispatcher;
extern eventpp::EventDispatcher<std::string, void(const std::string&)> gbutton_input_event_dispatcher;


#warning wip
// ArucoDetectorStatus aruco_detector_status;
extern std::map<std::string, int> thread_time_;
extern std::deque<DroneState> status_;
extern std::deque<cv::Mat> frames_;
extern std::vector<int> pid_values_;

extern std::vector<int> x_cmds;
extern std::vector<int> y_cmds;
extern std::vector<int> z_cmds;
extern std::vector<int> yaw_cmds;

#endif  // GLOBAL_H