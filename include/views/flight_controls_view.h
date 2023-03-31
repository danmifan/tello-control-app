#ifndef FLIGHT_CONTROL_VIEW_H
#define FLIGHT_CONTROL_VIEW_H

#include "window/aview.h"

#include "flight_control.h"
#include "video_streaming.h"

class FlightControlView : public AView {
 public:
  FlightControlView(FlightControl* fc, VideoStreaming* vs);
  void update() override;

 private:
  FlightControl* fc_;
  VideoStreaming* vs_;
  bool* joystick_enabled_;
};

#endif  // FLIGHT_CONTROL_VIEW_H