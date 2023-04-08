#ifndef FLIGHT_CONTROL_VIEW_H
#define FLIGHT_CONTROL_VIEW_H

#include "window/aview.h"

#include "flight_control.h"
#include "video_streaming.h"
#include "image_processing.h"

class FlightControlView : public AView {
 public:
  FlightControlView(FlightControl* fc, VideoStreaming* vs, ImageProcessing* ip);
  void update() override;

 private:
  FlightControl* fc_;
  VideoStreaming* vs_;
  ImageProcessing* ip_;
  bool* joystick_enabled_;
};

#endif  // FLIGHT_CONTROL_VIEW_H