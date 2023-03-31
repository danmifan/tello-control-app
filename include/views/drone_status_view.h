#ifndef DRONE_STATUS_VIEW_H
#define DRONE_STATUS_VIEW_H

#include "window/aview.h"

#include "global.h"

class DroneStatusView : public AView {
 public:
  void update() override;

 private:
  DroneState current_state_;
};

#endif  // DRONE_STATUS_VIEW_H