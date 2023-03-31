#ifndef ARUCO_DETECTOR_H
#define ARUCO_DETECTOR_H

#include <opencv2/aruco.hpp>

#define RAD_TO_DEG 180 / M_PI;

class ArucoDetector {
 public:
  ArucoDetector();
  void detect(cv::Mat frame);

 private:
  // Aruco
  cv::Mat_<double> camera_matrix_;
  cv::Mat_<double> dist_;
};

#endif  // ARUCO_DETECTOR_H