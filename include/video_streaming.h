#ifndef VIDEO_STREAMING_H
#define VIDEO_STREAMING_H

#include <atomic>
#include <thread>

class VideoStreaming {
 public:
  ~VideoStreaming();
  void init();

 private:
  std::atomic<bool> run_ = {true};
  std::thread th_;
};

#endif  // VIDEO_STREAMING_H