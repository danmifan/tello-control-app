#ifndef VIDEO_STREAMING_H
#define VIDEO_STREAMING_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <deque>
#include <atomic>
#include <vector>

class VideoStreaming {
 public:
  ~VideoStreaming();
  void init();

 private:
  struct sockaddr_in video_addr_;
  unsigned int source_addr_size_;
  int video_socket_;
};

#endif  // VIDEO_STREAMING_H