#ifndef DRONE_STATUS_H
#define DRONE_STATUS_H

#include "data.h"

#include <thread>
#include <deque>
#include <atomic>

class DroneStatus {
 public:
  ~DroneStatus();
  int start();

 private:
  std::deque<DroneState> states_;
  int state_socket_;
  std::thread th_;
  std::atomic<bool> run_ = {true};
  bool first_ = true;
  DroneState first_state_;
};

#endif  // DRONE_STATUS_H