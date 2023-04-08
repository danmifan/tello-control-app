#ifndef DRONE_STATUS_VIEW_H
#define DRONE_STATUS_VIEW_H

#include "window/aview.h"

#include "drone_status.h"

#include "global.h"

class DroneStatusView : public AView {
 public:
  DroneStatusView(DroneStatus* drone_status);
  void update() override;

 private:
  DroneState current_state_;
  DroneStatus* drone_status_;
};

#endif  // DRONE_STATUS_VIEW_H