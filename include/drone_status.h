#ifndef DRONE_STATUS_H
#define DRONE_STATUS_H

#include "data.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <deque>
#include <atomic>

class DroneStatus {
 public:
  ~DroneStatus();
  int start();

 private:
  std::deque<DroneState> states_;
  struct sockaddr_in state_addr_, source_addr_;
  int state_socket_;
  std::thread th_;
  std::atomic<bool> run_ = {false};
};

#endif  // DRONE_STATUS_H