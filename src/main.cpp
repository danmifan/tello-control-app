#include <iostream>
#include <signal.h>

#include "window/window.h"
#include "flight_control.h"
#include "drone_status.h"
#include "data.h"

bool run = true;

void handle_signal(int signal) { run = false; }

int main(int argc, char **argv) {
  MyWindow window(800, 600);

  signal(SIGINT, handle_signal);

  FlightControl flight_control;
  DroneStatus drone_status;

  // struct sockaddr_in video_in;
  // int video_socket;

  // struct timeval tv;
  // tv.tv_sec = 2;
  // tv.tv_usec = 0;

  // video_socket = socket(AF_INET, SOCK_DGRAM, 0);
  // if (video_socket == -1) {
  //   std::cout << "Errour could not initialize VIDEO socket" << std::endl;
  //   return -1;
  // }

  // video_in.sin_family = AF_INET;
  // video_in.sin_addr.s_addr = inet_addr("0.0.0.0");
  // video_in.sin_port = htons(11111);
  // if (bind(video_socket, (sockaddr *)&video_in, sizeof(video_in)) == -1) {
  //   std::cout << "VIDEO socket bind failed" << std::endl;
  // }

  flight_control.init();
  drone_status.init();

  while (run) {
    flight_control.takeoff();
    flight_control.rotate(10);
  }

  window.init();
  window.update();
  window.shutdown();

  return 0;
}