#include "image_processing.h"

#include <thread>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>

#include "logger.h"
#include "global.h"
#include "utils.h"

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
  tracker_ = cv::TrackerKCF::create();
  th_ = std::thread([&]() {
    while (run_) {
      auto t1 = std::chrono::high_resolution_clock::now();
      if (!frames_.empty()) {
        cv::Mat frame = frames_.back();
        frames_.pop_back();

        if (event_->active) {
          int tl_x = event_->data.x - rect_width / 2;
          int tl_y = event_->data.y - rect_height / 2;

          cv::Rect2d rect(tl_x, tl_y, rect_width, rect_height);

          cv::rectangle(frame, rect, cv::Scalar(255, 255, 255, 255));

          delta_pos_.x = -(960 / 2 - event_->data.x);
          delta_pos_.y = -(720 / 2 - event_->data.y);
          event_->active = false;
          // Log::get().info(std::to_string(delta_pos_.x) + " " + std::to_string(delta_pos_.y));

          if (!tracker_running_) {
            tracker_->init(frame, rect);
            tracker_running_ = true;
          } else {
            tracker_->clear();
            tracker_ = cv::TrackerKCF::create();
            tracker_->init(frame, rect);
          }
        }

        if (tracker_running_) {
          cv::Rect2d tracked_object;

          if (tracker_->update(frame, tracked_object)) {
            cv::rectangle(frame, tracked_object, cv::Scalar(0, 255, 0, 255));

            cv::circle(frame, cv::Point(960 / 2, 720 / 2), 10, cv::Scalar(255, 0, 0, 255));

            int target_center_x = tracked_object.x + tracked_object.width / 2;
            int target_center_y = tracked_object.y + tracked_object.height / 2;

            cv::circle(frame, cv::Point(target_center_x, target_center_y), 10,
                       cv::Scalar(255, 255, 255, 255));

            int dx = target_center_x - (960 / 2);
            int dy = target_center_y - (720 / 2);

            // Log::get().info("dx : " + std::to_string(dx) + " dy : " + std::to_string(dy));

            dx = clamp(dx, -100, 100);

            fc_->radioControl(0, 0, 0, dx);
          }
        }

        memcpy(image_, frame.data, 960 * 720 * 3);
      }

      auto t2 = std::chrono::high_resolution_clock::now();
      int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

      // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
}

void ImageProcessing::setEvent(Event* event) { event_ = event; }

unsigned char* ImageProcessing::getImage() { return image_; }
