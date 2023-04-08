#ifndef ARUCO_DETECTOR_H
#define ARUCO_DETECTOR_H

#include <opencv2/aruco.hpp>

#define RAD_TO_DEG 180 / M_PI;

class ArucoDetector {
 public:
  ArucoDetector();
  bool detect(cv::Mat frame, cv::Vec3d& mvt);

 private:
  // Aruco
  cv::Mat_<double> camera_matrix_;
  cv::Mat_<double> dist_;
  cv::Ptr<cv::aruco::GridBoard> board_;
  cv::Ptr<cv::aruco::Dictionary> dictionary_;
  cv::Ptr<cv::aruco::DetectorParameters> parameters_;
};

#endif  // ARUCO_DETECTOR_H