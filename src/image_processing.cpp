#include "image_processing.h"

#include <thread>

#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>

#include "data.h"
#include "logger.h"
#include "global.h"
#include "utils.h"
#include "pid_controller.h"

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
    PIDController z_pid(0.7, 0.0001, 0.1);
    PIDController yaw_pid(0.7, 0.0001, 0.1);

    while (run_) {
      auto t1 = std::chrono::high_resolution_clock::now();
      if (!frames_.empty()) {
        cv::Mat frame = frames_.back();
        frames_.pop_back();

        // Face detection
        // face_detection_.detect(frame);

        // Aruco
        // aruco_detector_.detect(frame);

        // Track
        TrackData target;
        if (tracker_.track(frame, target)) {
          // Correct

#warning Sortir PID de la condition
          float z_cmd = z_pid.correct(720 / 2, target.position.y);
          float yaw_cmd = yaw_pid.correct(960 / 2, target.position.x);

          int z_cmd_i = z_cmd;
          int yaw_cmd_i = yaw_cmd;

          Log::get().info("PID : " + std::to_string(z_cmd) + " " + std::to_string(yaw_cmd));

          // int yaw_cmd = target.dx * 0.5;
          // int z_cmd = -target.dy * 0.5;

          // int yaw_cmd = target.dx * 0.75;
          // int z_cmd = -target.dy * 0.75;

          // fc_->radioControl(0, 0, z_cmd, yaw_cmd);

          // file_.write(yaw_cmd, -z_cmd, target.dx, target.dy);
        }

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

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

      // Log::get().info("Imgproc : " + std::to_string(total_time));
    }
  });
}

void ImageProcessing::setEvent(Event* event) { tracker_.setEvent(event); }

unsigned char* ImageProcessing::getImage() { return image_; }

void ImageProcessing::setHoverPose() {}