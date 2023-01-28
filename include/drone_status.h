#ifndef DRONE_STATUS_H
#define DRONE_STATUS_H

#include "data.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <deque>

class DroneStatus {
 public:
  ~DroneStatus();
  int init();

 private:
  std::deque<DroneState> states_;
  struct sockaddr_in state_addr_, source_addr_;
  int state_socket_;
  std::thread th_;
};

#endif  // DRONE_STATUS_H