#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <atomic>
#include <thread>

#include "flight_control.h"
#include "data.h"

#define HFOV 50

class ImageProcessing {
 public:
  ImageProcessing(FlightControl* fc);
  ~ImageProcessing();
  void start();
  void setEvent(Event* event);
  unsigned char* getImage();

 private:
  unsigned char* image_ = nullptr;
  cv::Mat mat_;
  std::thread th_;
  std::atomic<bool> run_ = {false};
  Event* event_;
  int rect_width = 160;
  int rect_height = 100;
  Vec2i delta_pos_;
  cv::Ptr<cv::Tracker> tracker_;
  bool tracker_running_ = false;
  FlightControl* fc_;
};

#endif  // IMAGE_PROCESSING_H