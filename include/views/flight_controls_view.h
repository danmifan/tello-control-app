#ifndef FLIGHT_CONTROL_VIEW_H
#define FLIGHT_CONTROL_VIEW_H

#include "window/aview.h"

class FlightControlView : public AView {
 public:
  FlightControlView();
  void update() override;

 private:
  bool* joystick_enabled_;
};

#endif  // FLIGHT_CONTROL_VIEW_H