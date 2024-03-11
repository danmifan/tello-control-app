#include "image_processing.h"

#include <thread>

#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>

#include "data.h"
#include "logger.h"
#include "global.h"
#include "pid_controller.h"
#include "event.h"

std::vector<int> pid_values_;

std::vector<int> x_cmds;
std::vector<int> y_cmds;
std::vector<int> z_cmds;
std::vector<int> yaw_cmds;

ImageProcessing::ImageProcessing(int width, int height, int channels) {
  image_ = (unsigned char *)malloc(width * height * channels);

  pid_values_.reserve(10000);
  x_cmds.reserve(10000);
  y_cmds.reserve(10000);
  z_cmds.reserve(10000);
  yaw_cmds.reserve(10000);

  width_ = width;
  height_ = height;
  channels_ = channels;

  // slam_ = new ORB_SLAM3::System("/home/vincent/workspace/ORB_SLAM3/Vocabulary/ORBvoc.txt",
  //                               "cfg/calib.yaml", ORB_SLAM3::System::MONOCULAR, false);
}

ImageProcessing::~ImageProcessing() {
  if (run_) {
    run_ = false;
    th_.join();
  }
}

void ImageProcessing::start() {
  run_ = true;

  gevent_dispatcher.appendListener("IP_Hover", [&](const Event &) {
    hover();
    Log::get().info("Hover activated");
  });

  gevent_dispatcher.appendListener(
      "FaceDetector",
      eventpp::argumentAdapter<void(const EnableButtonEvent &)>([&](const EnableButtonEvent &e) {
        Log::get().info("Face detector");
        face_detector_enabled_ = e.enable;
      }));

  gevent_dispatcher.appendListener(
      "ArucoDetector",
      eventpp::argumentAdapter<void(const EnableButtonEvent &)>([&](const EnableButtonEvent &e) {
        Log::get().info("Aruco detector");
        aruco_detector_enabled_ = e.enable;
      }));

  th_ = std::thread([&]() {
    // PIDController z_pid(0.7, 0.0001, 0.1);
    PIDController z_pid(0.5, 0, 0);
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

        // float image_scale = slam_->GetImageScale();

        // if (image_scale != 1.f) {
        //   cv::Mat resized;
        //   int width = frame.cols * image_scale;
        //   int height = frame.rows * image_scale;

        //   cv::resize(resized, frame, cv::Size(width, height));
        // }

        double timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
                               std::chrono::high_resolution_clock::now().time_since_epoch())
                               .count();

        // slam_->TrackMonocular(frame, timestamp);

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

              RCEvent rc_event{(int)y_cmd, 0, (int)-z_cmd, 0};
              gevent_dispatcher.dispatch("RCCommands", rc_event);
            }
          }
        }

        // Track
        TrackData target;
        if (tracker_.track(frame, target)) {
          // Correct
          int z_cmd = z_pid.correct(height_ / 2.0f, target.position.y);
          int yaw_cmd = yaw_pid.correct(width_ / 2.0f, target.position.x);

          pid_values_.push_back(yaw_cmd);

          Log::get().info("PID : " + std::to_string(z_cmd) + " " + std::to_string(yaw_cmd));

          // int yaw_cmd = target.dx * 0.5;
          // int z_cmd = -target.dy * 0.5;

          // int yaw_cmd = target.dx * 0.75;
          // int z_cmd = -target.dy * 0.75;

          RCEvent rc_event{0, 0, -z_cmd, yaw_cmd};

          gevent_dispatcher.dispatch("RCCommands", rc_event);

          // file_.write(yaw_cmd, -z_cmd, target.dx, target.dy);
        }

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        memcpy(image_, frame.data, width_ * height_ * channels_);
      }

      auto t2 = std::chrono::high_resolution_clock::now();
      int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

      int delta = 10 - duration_ms;

      if (delta > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delta));
      }

      auto t3 = std::chrono::high_resolution_clock::now();
      int total_time = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t1).count();

      ThreadTimeEvent event("ImgProc", total_time);
      gevent_dispatcher.dispatch("ThreadTime", event);
    }
  });
}

unsigned char *ImageProcessing::getImage() { return image_; }

void ImageProcessing::hover() {
  hover_enabled_ = !hover_enabled_;
  if (hover_enabled_) {
    first_aruco_pose_ = false;
  }
}