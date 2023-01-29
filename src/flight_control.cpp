#include "flight_control.h"

#include <unistd.h>
#include <string.h>
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

  // Socket timeout
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  // setsockopt(cmd_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

  cmd_addr_.sin_family = AF_INET;
  cmd_addr_.sin_addr.s_addr = inet_addr("192.168.10.1");
  cmd_addr_.sin_port = htons(8889);

  source_addr_size_ = sizeof(source_addr_);

  th_ = std::thread([&]() {
    while (run_) {
      if (!commands_.empty()) {
        std::string command = commands_.back();
        commands_.pop_back();

        char *answer_buffer = (char *)malloc(256);

        std::cout << "Trying to send command..." << std::endl;
        if (sendto(cmd_socket_, command.c_str(), command.size(), 0, (struct sockaddr *)&cmd_addr_,
                   sizeof(cmd_addr_)) == -1) {
          std::cout << "Send failed" << std::endl;
        } else {
          std::cout << "Command sent : " << command << std::endl;
        }

        int answer_size = 0;

        do {
          answer_size = recvfrom(cmd_socket_, answer_buffer, 256, MSG_DONTWAIT,
                                 (struct sockaddr *)&source_addr_, &source_addr_size_);

        } while (answer_size <= 0);

        char *test_answer = (char *)malloc(answer_size);
        memcpy(test_answer, answer_buffer, answer_size);
        std::cout << "Answer size : " << answer_size << std::endl;
        std::cout << "Answer : " << test_answer << std::endl;
        delete test_answer;
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

void FlightControl::radioControl(int x, int y, int z, int yaw) {
  std::string cmd = "rc ";
  cmd += std::to_string(y) + " " + std::to_string(x) + " " + std::to_string(z) + " " +
         std::to_string(yaw);
  commands_.emplace_front(cmd);
}

void FlightControl::getBattery() { commands_.emplace_front(std::string("battery?")); }

void FlightControl::customCommand(std::string command) { commands_.emplace_front(command); }

void FlightControl::streamon() { commands_.emplace_front(std::string("streamon")); }
