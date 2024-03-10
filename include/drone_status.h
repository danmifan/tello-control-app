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
  int state_socket_;
  std::thread th_;
  std::atomic<bool> run_ = {true};
  bool first_ = true;
  DroneState first_state_;

  int key_;
  int shmid_;
  void* shm_addr_;
};

#endif  // DRONE_STATUS_H