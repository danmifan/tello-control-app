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
std::vector<int> pid_values_;

std::vector<int> x_cmds;
std::vector<int> y_cmds;
std::vector<int> z_cmds;
std::vector<int> yaw_cmds;

ImageProcessing::ImageProcessing(FlightControl* fc) {
  fc_ = fc;
  image_ = (unsigned char*)malloc(960 * 720 * 3);

  pid_values_.reserve(10000);
  x_cmds.reserve(10000);
  y_cmds.reserve(10000);
  z_cmds.reserve(10000);
  yaw_cmds.reserve(10000);
}

ImageProcessing::~ImageProcessing() {
  if (run_) {
    run_ = false;
    th_.join();
  }
}

void ImageProcessing::start() {
  run_ = true;

  gbutton_event_dispatcher.appendListener("IP_Hover", [&]() {
    hover();
    Log::get().info("Hover activated");
  });

   gbutton_event_dispatcher.appendListener("ArucoDetector", [&]() {
    Log::get().info("Aruco detector");
    aruco_detector_enabled_ = !aruco_detector_enabled_;
  });

   gbutton_event_dispatcher.appendListener("FaceDetector", [&]() {
    Log::get().info("Face detector");
    face_detector_enabled_ = !face_detector_enabled_;
  });


  th_ = std::thread([&]() {
    // PIDController z_pid(0.7, 0.0001, 0.1);
    PIDController z_pid(1, 0, 0);
    PIDController yaw_pid(0.5, 0, 0);
    PIDController y_pid(1, 0, 0);

    while (run_) {
      auto t1 = std::chrono::high_resolution_clock::now();
      if (!frames_.empty()) {
        cv::Mat frame = frames_.back();
        frames_.pop_back();

        // Face detection
        if (face_detector_enabled_) {
          face_detection_.detect(frame);
        }

        // Aruco
        if (aruco_detector_enabled_) {
          cv::Vec3d mvt = {0, 0, 0};
          if (aruco_detector_.detect(frame, mvt)) {
            if (hover_enabled_) {
              cv::putText(frame, "Hover", cv::Point2i(0, 600), cv::FONT_HERSHEY_PLAIN, 2,
                          cv::Scalar(255, 255, 255, 255));

              std::stringstream ss;
              ss << hover_pose_;

              cv::putText(frame, ss.str(), cv::Point2i(0, 650), cv::FONT_HERSHEY_PLAIN, 2,
                          cv::Scalar(255, 255, 255, 255));

              std::stringstream ss2;
              ss2 << mvt;
              cv::putText(frame, ss2.str(), cv::Point2i(0, 700), cv::FONT_HERSHEY_PLAIN, 2,
                          cv::Scalar(255, 255, 255, 255));

              if (!first_aruco_pose_) {
                hover_pose_ = mvt;
                first_aruco_pose_ = true;
              }
              float y_cmd = -y_pid.correct(mvt[1], hover_pose_[1]);
              float z_cmd = -z_pid.correct(mvt[2], hover_pose_[2]);

              y_cmds.push_back(y_cmd);
              z_cmds.push_back(z_cmd);

              // Log::get().info("PID : " + std::to_string(y_cmd) + " " + std::to_string(z_cmd));

              fc_->radioControl(y_cmd, 0, -z_cmd, 0);
            }
          }
        }

        // Track
        TrackData target;
        if (tracker_.track(frame, target)) {
          // Correct
          int z_cmd = z_pid.correct(720 / 2, target.position.y);
          int yaw_cmd = yaw_pid.correct(960 / 2, target.position.x);

          pid_values_.push_back(yaw_cmd);

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

void ImageProcessing::hover() {
  hover_enabled_ = !hover_enabled_;
  if (hover_enabled_) {
    first_aruco_pose_ = false;
  }
}

void ImageProcessing::enableArucoDetector(bool enable) { aruco_detector_enabled_ = enable; }

void ImageProcessing::enableFaceDetector(bool enable) { face_detector_enabled_ = enable; }