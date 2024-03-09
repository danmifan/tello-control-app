#include "drone_status.h"

#include <unistd.h>
#include <iostream>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>

// shm
#include <sys/ipc.h>
#include <sys/shm.h>

#include "global.h"
#include "logger.h"

std::deque<DroneState> status_;

DroneStatus::~DroneStatus() {
  close(state_socket_);
  run_ = false;
  th_.join();
}

int DroneStatus::start() {
  struct sockaddr_in state_addr_, source_addr_;

  state_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (state_socket_ == -1) {
    Log::get().warning("Error could not initialize STATE socket");
    return -1;
  }

  // Socket timeout
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  setsockopt(state_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

  state_addr_.sin_family = AF_INET;
  state_addr_.sin_addr.s_addr = inet_addr("0.0.0.0");
  state_addr_.sin_port = htons(8890);

  if (bind(state_socket_, (struct sockaddr *)&state_addr_, sizeof(state_addr_)) == -1) {
    Log::get().warning("STATE socket bind failed");
    return -1;
  }

  // Creating shm
  key_ = ftok("/tmp", 'R');
  Log::get().info(std::to_string(key_));
  shmid_ = shmget(key_, sizeof(DroneState), 0644 | IPC_CREAT);
  Log::get().info(std::to_string(shmid_));
  shm_addr_ = shmat(shmid_, NULL, 0);

  th_ = std::thread([&]() {
    while (run_) {
      auto t1 = std::chrono::high_resolution_clock::now();

      char state_buffer[256];
      unsigned int len = sizeof(source_addr_);

      int size = recvfrom(state_socket_, state_buffer, sizeof(state_buffer), 0,
                          (struct sockaddr *)&source_addr_, &len);

      if (size < 0) {
        // std::cout << "State reception failed" << std::endl;
      } else {
        DroneState state;

        struct timeval tv;
        gettimeofday(&tv, NULL);
        state.timestamp = tv;

        sscanf(state_buffer,
               "pitch:%d;roll:%d;yaw:%d;vgx:%d;vgy:%d;vgz:%d;templ:%d;temph:%d;tof:%d;h:%d;bat:%d;"
               "baro:%f;time:%d;agx:%f;agy:%f;agz:%f;",
               &state.attitude.y, &state.attitude.x, &state.attitude.z, &state.velocity.x,
               &state.velocity.y, &state.velocity.z, &state.templ, &state.temph, &state.tof,
               &state.h, &state.bat, &state.baro, &state.time, &state.acceleration.x,
               &state.acceleration.y, &state.acceleration.z);

        if (first_) {
          first_state_ = state;
          first_ = false;
        } else {
          state.dbaro = state.baro - first_state_.baro;
        }

        auto t2 = std::chrono::high_resolution_clock::now();
        // int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        float duration_s =
            std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count();

        state.dt = duration_s;

        status_.push_back(state);

        float aX = state.acceleration.y;
        float aY = state.acceleration.x;
        float aZ = -state.acceleration.z;

        float pitch = atan2(aY, aZ) * 57.3;
        float roll = atan2((-aX), sqrt(aY * aY + aZ * aZ)) * 57.3;

        std::cout << "Roll pitch " << roll << " " << pitch << std::endl;

        std::cout << state.attitude.x << " " << state.attitude.y << std::endl;

        // state.acceleration.x = roll;
        // state.acceleration.y = pitch;

        // Writing data to shm
        memcpy(shm_addr_, &state, sizeof(DroneState));
      }
    }
  });

  return 1;
}
