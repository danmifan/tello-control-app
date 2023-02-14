#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <thread>
#include <chrono>

bool run = true;

void signalHandler(int signum) {
  std::cout << "SIGINT caught" << std::endl;
  run = false;
}

int main(int argc, char **argv) {
  signal(SIGINT, signalHandler);

  int cmd_socket_;
  sockaddr_in cmd_addr_, source_addr_;
  unsigned int source_addr_size_;
  cmd_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (cmd_socket_ == -1) {
    std::cout << "Error, could not initialize CMD socket" << std::endl;
    return -1;
  }

  sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(8889);

  if (bind(cmd_socket_, (sockaddr *)&addr, sizeof(addr)) == -1) {
    std::cout << "CMD bind failed" << std::endl;
  }

  cmd_addr_.sin_family = AF_INET;
  cmd_addr_.sin_addr.s_addr = inet_addr("192.168.10.1");
  cmd_addr_.sin_port = htons(8889);

  source_addr_size_ = sizeof(source_addr_);

  // Socket timeout
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  setsockopt(cmd_socket_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

  const int enable = 1;
  setsockopt(cmd_socket_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

  while (run) {
    // std::string command;

    // std::cout << ">";
    // std::cin >> command;

    std::cout << "Trying to send command..." << std::endl;

    char command[256] = "command\0";

    int send_size =
        sendto(cmd_socket_, command, 8, 0, (struct sockaddr *)&cmd_addr_, sizeof(cmd_addr_));

    std::cout << send_size << std::endl;

    if (send_size > 0) {
      std::cout << "Command sent : " << command << std::endl;
    } else {
      std::cout << "Send failed" << std::endl;
    }

    std::string answer;
    answer.resize(512);

    int answer_size = recvfrom(cmd_socket_, &(answer[0]), answer.size(), 0,
                               (struct sockaddr *)&source_addr_, &source_addr_size_);

    if (answer_size < 0) {
      std::cout << "Nothing received" << std::endl;
    } else {
      // memcpy(test_answer, answer_buffer, answer_size);
      std::cout << "Answer size : " << answer_size << std::endl;
      std::cout << "Answer : " << answer << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  close(cmd_socket_);

  std::cout << "Bye" << std::endl;

  return 0;
}