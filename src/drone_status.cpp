#include "drone_status.h"

#include <unistd.h>
#include <iostream>

#include "global.h"

DroneStatus::~DroneStatus() {
  close(state_socket_);
  run_ = false;
  th_.join();
}

int DroneStatus::init() {
  state_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (state_socket_ == -1) {
    std::cout << "Error could not initialize STATE socket" << std::endl;
    return -1;
  }

  struct timeval tv;
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  setsockopt(state_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

  state_addr_.sin_family = AF_INET;
  state_addr_.sin_addr.s_addr = inet_addr("0.0.0.0");
  state_addr_.sin_port = htons(8890);

  if (bind(state_socket_, (struct sockaddr *)&state_addr_, sizeof(state_addr_)) == -1) {
    std::cout << "STATE socket bind failed" << std::endl;
    return -1;
  }

  th_ = std::thread([&]() {
    while (run_) {
      DroneState state;

      char state_buffer[256];
      unsigned int len = sizeof(source_addr_);

      int size = recvfrom(state_socket_, state_buffer, sizeof(state_buffer), 0,
                          (struct sockaddr *)&source_addr_, &len);

      if (size < 0) {
        // std::cout << "State reception failed" << std::endl;
      } else {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        state.timestamp = tv;

        time_t nowtime;
        char buffer[256];

        std::cout << state_buffer << std::endl;

        sscanf(state_buffer,
               "pitch:%i;roll:%i;yaw:%i;vgx:%f;vgy:%f;vgz:%f;templ:%i;temph:%i;tof:%i;h:%i;bat:%i;"
               "baro:%f;time:%i;agx:%f;agy:%f;agz:%f;",
               &state.attitude.x, &state.attitude.y, &state.attitude.z, &state.velocity.x,
               &state.velocity.y, &state.velocity.z, &state.templ, &state.temph, &state.tof,
               &state.h, &state.bat, &state.baro, &state.time, &state.acceleration.x,
               &state.acceleration.y, &state.acceleration.z);

        // states_.push_front(state);
        status_.push_back(state);
      }
    }
  });

  return 1;
}