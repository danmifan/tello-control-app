#include "flight_control.h"

#include <unistd.h>
#include <string.h>
#include <iostream>
#include "logger.h"

FlightControl::~FlightControl() {
  run_ = false;
  th_.join();
  close(cmd_socket_);
  if (answer_buffer_ != nullptr) {
    delete answer_buffer_;
  }
}

int FlightControl::start() {
  cmd_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (cmd_socket_ == -1) {
    Log::get().warning("Error, could not initialize CMD socket");
    return -1;
  }

  answer_buffer_ = (char *)malloc(256);

  // Socket timeout
  struct timeval tv;
  tv.tv_sec = COMMAND_TIMEOUT_SEC;
  tv.tv_usec = COMMAND_TIMEOUT_USEC;

  setsockopt(cmd_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

  cmd_addr_.sin_family = AF_INET;
  cmd_addr_.sin_addr.s_addr = inet_addr("192.168.10.1");
  cmd_addr_.sin_port = htons(8889);

  source_addr_size_ = sizeof(source_addr_);

  th_ = std::thread([&]() {
    while (run_) {
      auto t1 = std::chrono::high_resolution_clock::now();
      if (!commands_.empty()) {
        auto begin = std::chrono::high_resolution_clock::now();
        int cooldown = 0;

        time_since_last_command_ = 0;
        std::string command = commands_.back();
        commands_.pop_back();

        if (command.find("rc") != std::string::npos) {
          cooldown = RC_COOLDOWN_TIME;
        } else {
          cooldown = COOLDOWN_TIME;
        }

        int send_size = sendto(cmd_socket_, command.data(), command.size(), 0,
                               (struct sockaddr *)&cmd_addr_, sizeof(cmd_addr_));

        if (send_size < 0) {
          Log::get().warning("Send timeout");
          continue;
        } else {
          Log::get().info("Command sent : " + command);
        }

        int answer_size = recvfrom(cmd_socket_, answer_buffer_, 256, 0,
                                   (struct sockaddr *)&source_addr_, &source_addr_size_);

        if (answer_size < 0) {
          Log::get().warning("Recv timeout");
        } else {
          char *test_answer = (char *)malloc(answer_size);
          memcpy(test_answer, answer_buffer_, answer_size);
          Log::get().info("Answer : " + std::string(test_answer));

          delete test_answer;
        }

        auto end = std::chrono::high_resolution_clock::now();
        int elapsed_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        int delta = cooldown - elapsed_time;
        if (delta > 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(delta));
        }
      }

      auto t2 = std::chrono::high_resolution_clock::now();
      time_since_last_command_ +=
          std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

      // Tello disables itself after 15 seconds without command
      if (time_since_last_command_ > KEEP_ALIVE_TIME) {
        enableSDK();
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

void FlightControl::radioControl(int a, int b, int c, int d) {
  std::string cmd = "rc ";
  cmd += std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + " " +
         std::to_string(d);
  commands_.emplace_front(cmd);
}

void FlightControl::getBattery() { commands_.emplace_front(std::string("battery?")); }

void FlightControl::customCommand(std::string command) { commands_.emplace_front(command); }

void FlightControl::streamon() { commands_.emplace_front(std::string("streamon")); }

void FlightControl::streamoff() { commands_.emplace_front(std::string("streamoff")); }

void FlightControl::hover() { commands_.emplace_front(std::string("stop")); }

void FlightControl::emergencyStop() { commands_.emplace_front(std::string("emergency")); }

bool FlightControl::isFlying() { return is_flying_; }