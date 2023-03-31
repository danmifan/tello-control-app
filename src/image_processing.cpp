#include "image_processing.h"

#include <thread>

#include <opencv2/core/utility.hpp>

#include "data.h"
#include "logger.h"
#include "global.h"
#include "utils.h"

std::map<std::string, int> thread_time_;

ImageProcessing::ImageProcessing(FlightControl* fc) {
  fc_ = fc;
  image_ = (unsigned char*)malloc(960 * 720 * 3);
}

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
      auto t1 = std::chrono::high_resolution_clock::now();
      if (!frames_.empty()) {
        cv::Mat frame = frames_.back();
        frames_.pop_back();

        // track
        Vec2i target;
        if (tracker_.track(frame, target)) {
          // Correct
          target.x = clamp(target.x, -100, 100);
          target.y = clamp(target.y, -100, 100);

          int yaw_cmd = target.x * 0.5;
          int z = -target.y * 0.5;

          fc_->radioControl(0, 0, z, yaw_cmd);

          // pid_.correct();
        }
        // aruco
        // aruco_detector_.detect(frame);

        memcpy(image_, frame.data, 960 * 720 * 3);
      }

      auto t2 = std::chrono::high_resolution_clock::now();
      int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

      int delta = 10 - duration_ms;

      if (delta > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delta));
      }

      auto t3 = std::chrono::high_resolution_clock::now();
      int total_time = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t1).count();

      thread_time_["ImgProc"] = total_time;

      // Log::get().info("Imgproc : " + std::to_string(duration_ms));

      // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
}

void ImageProcessing::setEvent(Event* event) { tracker_.setEvent(event); }

unsigned char* ImageProcessing::getImage() { return image_; }

void ImageProcessing::setHoverPose() {}