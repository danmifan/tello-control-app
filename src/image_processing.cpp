#include "image_processing.h"

#include <thread>

#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

#include "logger.h"
#include "global.h"
#include "utils.h"

std::map<std::string, int> thread_time_;

cv::Vec3f decomposeRotation(cv::Mat m) {
  cv::Vec3f euler;

  double r31 = m.at<double>(2, 0);
  double r32 = m.at<double>(2, 1);
  double r33 = m.at<double>(2, 2);
  double r21 = m.at<double>(1, 0);
  double r11 = m.at<double>(0, 0);

  float x = atan2(r32, r33) * RAD_TO_DEG;
  float y = atan2(-r31, sqrt(r32 * r32 + r33 * r33)) * RAD_TO_DEG;
  float z = atan2(r21, r11) * RAD_TO_DEG;

  return {x, y, z};
}

ImageProcessing::ImageProcessing(FlightControl* fc) {
  fc_ = fc;
  image_ = (unsigned char*)malloc(960 * 720 * 3);

  camera_matrix_ = cv::Mat_<double>(3, 3);

  camera_matrix_ << 929.6872834493681, 0, 497.2396432652593, 0, 924.0825967784374, 320.3428, 0, 0,
      1;

  dist_ = cv::Mat_<double>(5, 1);

  dist_ << .03725270707206475, -0.4971384516604886, -0.003723941725012836, 0.003059745454830841,
      1.702533434357973;
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

        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
        cv::Ptr<cv::aruco::Dictionary> dictionary =
            cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
        cv::aruco::detectMarkers(frame, dictionary, markerCorners, markerIds, parameters,
                                 rejectedCandidates);
        if (markerIds.size() > 0) {
          cv::aruco::drawDetectedMarkers(frame, markerCorners);

          std::vector<cv::Vec3d> rvecs, tvecs;
          cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.066, camera_matrix_, dist_, rvecs,
                                               tvecs);

          // draw axis for each marker
          for (int i = 0; i < markerIds.size(); i++) {
            cv::aruco::drawAxis(frame, camera_matrix_, dist_, rvecs[i], tvecs[i], 0.1);

            cv::Mat m_rot;

            cv::Rodrigues(rvecs, m_rot);
            cv::Vec3f euler = decomposeRotation(m_rot);

            cv::Mat t = cv::Mat(cv::Size(4, 4), CV_64F);

            cv::Rect roi(0, 0, 3, 3);
            m_rot(roi).copyTo(t(roi));

            t.at<double>(0, 3) = tvecs[i][0];
            t.at<double>(1, 3) = tvecs[i][1];
            t.at<double>(2, 3) = tvecs[i][2];
            t.at<double>(3, 0) = 0;
            t.at<double>(3, 1) = 0;
            t.at<double>(3, 2) = 0;
            t.at<double>(3, 3) = 1;

            std::stringstream ss;
            ss << euler;

            cv::putText(frame, ss.str(), markerCorners.at(0).at(0), cv::FONT_HERSHEY_PLAIN, 2,
                        cv::Scalar(255, 255, 255, 255));

            ss = std::stringstream();
            ss << tvecs[i];

            t = t.inv();

            cv::Vec3d new_vec = {t.at<double>(0, 3), t.at<double>(1, 3), t.at<double>(2, 3)};

            cv::Point2f org = markerCorners.at(0).at(0);
            org.y += 50;

            cv::putText(frame, ss.str(), org, cv::FONT_HERSHEY_PLAIN, 2,
                        cv::Scalar(255, 255, 255, 255));

            std::cout << t << std::endl;
          }
        }

        // if (event_->active) {
        //   int tl_x = event_->data.x - rect_width / 2;
        //   int tl_y = event_->data.y - rect_height / 2;

        //   cv::Rect2d rect(tl_x, tl_y, rect_width, rect_height);

        //   cv::rectangle(frame, rect, cv::Scalar(255, 255, 255, 255));

        //   delta_pos_.x = -(960 / 2 - event_->data.x);
        //   delta_pos_.y = -(720 / 2 - event_->data.y);
        //   event_->active = false;
        //   // Log::get().info(std::to_string(delta_pos_.x) + " " +
        //   std::to_string(delta_pos_.y));

        //   if (!tracker_running_) {
        //     tracker_->init(frame, rect);
        //     tracker_running_ = true;
        //   } else {
        //     tracker_->clear();
        //     tracker_ = cv::TrackerKCF::create();
        //     tracker_->init(frame, rect);
        //   }
        // }

        // if (tracker_running_) {
        //   cv::Rect2d tracked_object;

        //   if (tracker_->update(frame, tracked_object)) {
        //     cv::rectangle(frame, tracked_object, cv::Scalar(0, 255, 0, 255));

        //     cv::circle(frame, cv::Point(960 / 2, 720 / 2), 10, cv::Scalar(255, 0, 0, 255));

        //     int target_center_x = tracked_object.x + tracked_object.width / 2;
        //     int target_center_y = tracked_object.y + tracked_object.height / 2;

        //     cv::circle(frame, cv::Point(target_center_x, target_center_y), 10,
        //                cv::Scalar(255, 255, 255, 255));

        //     int dx = target_center_x - (960 / 2);
        //     int dy = target_center_y - (720 / 2);

        //     // Log::get().info("dx : " + std::to_string(dx) + " dy : " + std::to_string(dy));

        //     dx = clamp(dx, -100, 100);

        //     int yaw_cmd = dx / 2;

        //     fc_->radioControl(0, 0, 0, yaw_cmd);
        //   }
        // }

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

void ImageProcessing::setEvent(Event* event) { event_ = event; }

unsigned char* ImageProcessing::getImage() { return image_; }
