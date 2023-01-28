#include "flight_control.h"

#include <unistd.h>
#include <iostream>

FlightControl::~FlightControl() {
  close(cmd_socket_);
  run_ = false;
  th_.join();
}

int FlightControl::init() {
  cmd_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (cmd_socket_ == -1) {
    std::cout << "Error, could not initialize CMD socket" << std::endl;
    return -1;
  }

  struct timeval tv;
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  setsockopt(cmd_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

  cmd_addr_.sin_family = AF_INET;
  cmd_addr_.sin_addr.s_addr = inet_addr("192.168.10.1");
  cmd_addr_.sin_port = htons(8889);

  source_addr_size_ = sizeof(source_addr_);

  enableSDK();

  char answer_buffer[256];

  th_ = std::thread([&]() {
    while (run_) {
      if (!commands_.empty()) {
        std::string command = commands_.back();
        commands_.pop_back();

        if (sendto(cmd_socket_, command.c_str(), command.size(), 0, (struct sockaddr *)&cmd_addr_,
                   sizeof(cmd_addr_)) == -1) {
          std::cout << "Send failed" << std::endl;
        } else {
          std::cout << "Command sent : " << command << std::endl;
        }

        int size = recvfrom(cmd_socket_, answer_buffer, sizeof(answer_buffer), 0,
                            (struct sockaddr *)&source_addr_, &source_addr_size_);
        if (size < 0) {
          // std::cout << "Answer reception failed" << std::endl;
        } else {
          // std::cout << " Address : " << cmd_in.sin_addr.s_addr << std::endl;
          std::cout << "Answer : " << answer_buffer << std::endl;
          std::cout << "Answer size : " << size << std::endl;
        }
      }
    }
  });

  return 1;
}

void FlightControl::enableSDK() { commands_.emplace_front(std::string("command")); }

void FlightControl::rotate(int angle) {
  std::string cmd;
  if (angle < 0) {
    cmd = "ccw " + std::to_string(std::abs(angle));
  } else {
    cmd = "cw " + std::to_string(angle);
  }

  commands_.push_front(cmd);
}

void FlightControl::takeoff() { commands_.emplace_front(std::string("takeoff")); }

void FlightControl::land() { commands_.emplace_front(std::string("land")); }
