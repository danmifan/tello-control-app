#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "data.h"
#include "window/abstract_widget.h"
#include "flight_control.h"

#include "global.h"

class MainView : public AbstractWidget {
 public:
  MainView(FlightControl* fc) : fc_(fc) {}
  void update() override;

 private:
  DroneState current_state_;
  FlightControl* fc_;
};

#endif  // MAIN_VIEW_H