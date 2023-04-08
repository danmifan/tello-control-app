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
  tv_.tv_sec = COMMAND_TIMEOUT_SEC;
  tv_.tv_usec = COMMAND_TIMEOUT_USEC;

  rc_tv_.tv_sec = RC_COMMAND_TIMEOUT_SEC;
  rc_tv_.tv_usec = RC_COMMAND_TIMEOUT_USEC;

  cmd_addr_.sin_family = AF_INET;
  cmd_addr_.sin_addr.s_addr = inet_addr("192.168.10.1");
  cmd_addr_.sin_port = htons(8889);

  source_addr_size_ = sizeof(source_addr_);

  th_ = std::thread([&]() {
    while (run_) {
      auto th_begin = std::chrono::high_resolution_clock::now();
      int cooldown = RC_COOLDOWN_TIME;

      // Tello disables itself after 15 seconds without command
      if (time_since_last_command_ > KEEP_ALIVE_TIME) {
        Log::get().info("Keeping connection alive...");
        enableSDK();
      }

      if (!commands_.empty()) {
        time_since_last_command_ = 0;
        std::string command = commands_.back();
        commands_.pop_back();

        // Not a RC command
        if (command.find("rc") == std::string::npos) {
          cooldown = COOLDOWN_TIME;
          // Probably not the best way to do this
          setsockopt(cmd_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv_, sizeof(tv_));
        } else {
          // No answer is expected when sending RC commands
          setsockopt(cmd_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&rc_tv_, sizeof(rc_tv_));
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
      }

      auto logic_end = std::chrono::high_resolution_clock::now();
      int total_logic =
          std::chrono::duration_cast<std::chrono::milliseconds>(logic_end - th_begin).count();

      int delta = cooldown - total_logic;
      if (delta > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delta));
      }

      auto th_end = std::chrono::high_resolution_clock::now();
      int total_th =
          std::chrono::duration_cast<std::chrono::milliseconds>(th_end - th_begin).count();

      time_since_last_command_ += total_th;
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

void FlightControl::land() {
  // Emergency command have a higher priority
  commands_.emplace_back(std::string("land"));
  is_flying_ = false;
}

void FlightControl::radioControl(int y, int x, int minus_z, int yaw) {
  std::string cmd = "rc ";
  cmd += std::to_string(y) + " " + std::to_string(x) + " " + std::to_string(minus_z) + " " +
         std::to_string(yaw);
  commands_.emplace_front(cmd);
}

void FlightControl::getBattery() { commands_.emplace_front(std::string("battery?")); }

void FlightControl::customCommand(std::string command) { commands_.emplace_front(command); }

void FlightControl::streamon() { commands_.emplace_front(std::string("streamon")); }

void FlightControl::streamoff() { commands_.emplace_front(std::string("streamoff")); }

void FlightControl::emergencyStop() {
  // Emergency command have a higher priority
  commands_.emplace_back(std::string("emergency"));
  is_flying_ = false;
}

bool FlightControl::isFlying() { return is_flying_; }
