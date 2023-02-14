#include "flight_control.h"

#include <unistd.h>
#include <string.h>
#include <iostream>

FlightControl::~FlightControl() {
  run_ = false;
  th_.join();
  close(cmd_socket_);
}

int FlightControl::init() {
  cmd_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (cmd_socket_ == -1) {
    std::cout << "Error, could not initialize CMD socket" << std::endl;
    return -1;
  }

  // Socket timeout
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  setsockopt(cmd_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

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

        std::cout << "Sending command... " << command << std::endl;

        int send_size = sendto(cmd_socket_, command.data(), command.size(), 0,
                               (struct sockaddr *)&cmd_addr_, sizeof(cmd_addr_));

        if (send_size < 0) {
          std::cout << "Send timeout " << std::endl;
          continue;
        } else {
          std::cout << "Command sent : " << command << std::endl;
        }

        int answer_size = recvfrom(cmd_socket_, answer_buffer, 256, 0,
                                   (struct sockaddr *)&source_addr_, &source_addr_size_);

        if (answer_size < 0) {
          std::cout << "Recv timeout" << std::endl;
        } else {
          char *test_answer = (char *)malloc(answer_size);
          memcpy(test_answer, answer_buffer, answer_size);
          std::cout << "Answer size : " << answer_size << std::endl;
          std::cout << "Answer : " << test_answer << std::endl;

          if (strcmp(test_answer, "ok") == 0) {
            std::cout << "bien ouej" << std::endl;
            if (command == "command") {
              std::cout << "Command AAAA" << std::endl;
            }
          } else if (strcmp(test_answer, "error") == 0) {
          }
          delete test_answer;

          // The drone needs time between two commands
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

void FlightControl::takeoff() {
  commands_.emplace_front(std::string("takeoff"));
  is_flying_ = true;
}

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

void FlightControl::streamoff() { commands_.emplace_front(std::string("streamoff")); }

bool FlightControl::isFlying() { return is_flying_; }