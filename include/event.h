#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include <eventpp/eventdispatcher.h>
#include <eventpp/utilities/argumentadapter.h>

#include "data.h"

struct Event {};

struct ButtonInputEvent : public Event {
  ButtonInputEvent(std::string input) : input(input) {}
  std::string input;
};

struct EnableButtonEvent : public Event {
  EnableButtonEvent(bool enable) : enable(enable) {}
  bool enable;
};

struct RCEvent : public Event {
  RCEvent(int y, int x, int minus_z, int yaw) : y(y), x(x), minus_z(minus_z), yaw(yaw) {}
  int y;
  int x;
  int minus_z;
  int yaw;
};

struct MouseEvent : public Event {
  MouseEvent(int x, int y) : x(x), y(y) {}
  int x;
  int y;
};

struct DroneStateEvent : public Event {
  DroneStateEvent(DroneState state) : state(state) {}
  DroneState state;
};

struct ArucoDetectorStatusEvent : public Event {
  std::vector<ArucoMarker> markers;
};

extern eventpp::EventDispatcher<std::string, void(const Event&)> gevent_dispatcher;

#endif  // EVENT_H