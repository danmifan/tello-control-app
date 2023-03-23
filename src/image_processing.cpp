#include "image_processing.h"

#include <thread>
#include <opencv2/imgproc.hpp>
#include "logger.h"
#include "global.h"

ImageProcessing::ImageProcessing() { image_ = (unsigned char*)malloc(960 * 720 * 3); }

ImageProcessing::~ImageProcessing() {
  if (run_) {
    run_ = false;
    th_.join();
  }
}

void ImageProcessing::start() {
  run_ = true;
  th_ = std::thread([&]() {
    while (run_) {
      if (!frames_.empty()) {
        cv::Mat frame = frames_.back();
        frames_.pop_back();

        if (event_->active) {
          int tl_x = event_->data.x - rect_width / 2;
          int tl_y = event_->data.y - rect_height / 2;

          cv::rectangle(frame, cv::Rect(tl_x, tl_y, rect_width, rect_height),
                        cv::Scalar(255, 255, 255, 255));

          delta_pos_.x = -(960 / 2 - event_->data.x);
          delta_pos_.y = -(720 / 2 - event_->data.y);
          event_->active = false;
          // Log::get().info(std::to_string(delta_pos_.x) + " " + std::to_string(delta_pos_.y));
        }
        memcpy(image_, frame.data, 960 * 720 * 3);
      }

      // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
}

void ImageProcessing::setImage(unsigned char* image) {
  // image_ = image;
  // mat_ = cv::Mat(cv::Size(960, 720), CV_8UC3, image);
}

void ImageProcessing::setEvent(Event* event) { event_ = event; }

unsigned char* ImageProcessing::getImage() { return image_; }
