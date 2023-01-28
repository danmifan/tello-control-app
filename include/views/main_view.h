#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "data.h"
#include "window/abstract_widget.h"

#include "global.h"

class MainView : public AbstractWidget {
 public:
  void update();

 private:
  DroneState current_state_;
};

#endif  // MAIN_VIEW_H