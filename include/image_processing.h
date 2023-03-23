#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <opencv2/core.hpp>
#include <atomic>
#include <thread>

#define HFOV 50

#include "data.h"

class ImageProcessing {
 public:
  ImageProcessing();
  ~ImageProcessing();
  void start();
  void setImage(unsigned char* image);
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
};

#endif  // IMAGE_PROCESSING_H