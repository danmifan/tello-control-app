#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <deque>
#include <atomic>
#include <vector>

#define COOLDOWN_TIME 100          // ms
#define RC_COOLDOWN_TIME 10        // ms
#define KEEP_ALIVE_TIME 10000      // ms
#define COMMAND_TIMEOUT_SEC 2      // s
#define COMMAND_TIMEOUT_USEC 0     // usec
#define RC_COMMAND_TIMEOUT_SEC 0   // s
#define RC_COMMAND_TIMEOUT_USEC 1  // usec

class FlightControl {
 public:
  ~FlightControl();
  int start();
  void shutdown();

  void rotate(int angle);
  void takeoff();
  void land();
  void radioControl(int x, int y, int z, int yaw);
  void enableSDK();
  void getBattery();
  void customCommand(std::string command);
  void streamon();
  void streamoff();
  void hover();
  void emergencyStop();
  bool isFlying();

 private:
  struct sockaddr_in cmd_addr_, source_addr_;
  unsigned int source_addr_size_;
  int cmd_socket_;
  std::deque<std::string> commands_;
  bool last_message_status_ = false;
  std::thread th_;
  std::atomic<bool> run_ = {true};
  bool is_flying_ = false;
  int time_since_last_command_ = 0;
  struct timeval tv_;
  struct timeval rc_tv_;

  char* answer_buffer_ = nullptr;
};

#endif  // FLIGHT_CONTROL_H