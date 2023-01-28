#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <deque>

class FlightControl {
 public:
  ~FlightControl();
  int init();

  void rotate(int angle);
  void takeoff();
  void land();

 private:
  struct sockaddr_in cmd_addr_, source_addr_;
  unsigned int source_addr_size_;
  int cmd_socket_;
  std::deque<std::string> commands_;
  bool last_message_status_ = false;
  bool sdk_mode_enabled_ = false;
  std::thread th_;

  void enableSDK();
};

#endif  // FLIGHT_CONTROL_H