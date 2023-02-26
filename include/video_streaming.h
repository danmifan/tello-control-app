#ifndef VIDEO_STREAMING_H
#define VIDEO_STREAMING_H

#include <atomic>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <deque>

class VideoStreaming {
 public:
  VideoStreaming(int width, int height, int channels);
  ~VideoStreaming();
  void start();
  void stop();

  unsigned char* getImage();

 private:
  std::atomic<bool> run_ = {false};
  std::thread th_;
  cv::VideoCapture cap_;
  std::deque<cv::Mat> frames_;
  unsigned char* image_;
};

#endif  // VIDEO_STREAMING_H