#include "aruco_detector.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

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

ArucoDetector::ArucoDetector() {
  camera_matrix_ = cv::Mat_<double>(3, 3);

  camera_matrix_ << 929.6872834493681, 0, 497.2396432652593, 0, 924.0825967784374, 320.3428, 0, 0,
      1;

  dist_ = cv::Mat_<double>(5, 1);

  dist_ << .03725270707206475, -0.4971384516604886, -0.003723941725012836, 0.003059745454830841,
      1.702533434357973;

  dictionary_ = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  board_ = cv::aruco::GridBoard::create(4, 6, 0.03, 0.007, dictionary_, 120);

  parameters_ = cv::aruco::DetectorParameters::create();
}

bool ArucoDetector::detect(cv::Mat frame, cv::Vec3d& mvt) {
  std::vector<int> marker_ids;
  std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;

  cv::aruco::detectMarkers(frame, dictionary_, marker_corners, marker_ids, parameters_,
                           rejected_candidates);

  // cv::aruco::refineDetectedMarkers(frame, board_, marker_corners, marker_ids,
  // rejected_candidates);

  if (marker_ids.size() > 0) {
    cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids);

    std::vector<cv::Vec3d> rvecs, tvecs;
    // cv::aruco::estimatePoseSingleMarkers(marker_corners, 0.03, camera_matrix_, dist_, rvecs,
    // tvecs);
    cv::aruco::estimatePoseSingleMarkers(marker_corners, 0.067, camera_matrix_, dist_, rvecs,
                                         tvecs);

    std::vector<cv::Vec3d> trans;

    cv::Vec3d eulers;

    for (uint i = 0; i < marker_ids.size(); i++) {
      cv::drawFrameAxes(frame, camera_matrix_, dist_, rvecs[i], tvecs[i], 0.05);

      cv::Mat m_rot;

      cv::Rodrigues(rvecs[i], m_rot);
      cv::Vec3f euler = decomposeRotation(m_rot);

      // cv::Mat t = cv::Mat(cv::Size(4, 4), CV_64F);

      cv::Rect roi(0, 0, 3, 3);
      // m_rot(roi).copyTo(t(roi));

      // t.at<double>(0, 3) = tvecs[i][0];
      // t.at<double>(1, 3) = tvecs[i][1];
      // t.at<double>(2, 3) = tvecs[i][2];
      // t.at<double>(3, 0) = 0;
      // t.at<double>(3, 1) = 0;
      // t.at<double>(3, 2) = 0;
      // t.at<double>(3, 3) = 1;

      std::stringstream euler_ss;
      euler_ss << euler;
      // std::cout << euler << std::endl;

      std::stringstream tvec_ss;
      tvec_ss << tvecs[i];
      std::cout << tvecs[i] << std::endl;

      cv::putText(frame, euler_ss.str(), marker_corners.at(0).at(0), cv::FONT_HERSHEY_PLAIN, 2,
                  cv::Scalar(255, 255, 255, 255));

      cv::putText(frame, tvec_ss.str(), marker_corners.at(0).at(0) + cv::Point2f(0, 50),
                  cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255, 255));

      euler_ss = std::stringstream();
      tvec_ss = std::stringstream();

      mvt[0] = tvecs[i][2] * 100;
      mvt[1] = tvecs[i][0] * 100;
      mvt[2] = tvecs[i][1] * 100;
    }

    return true;
  }

  return false;
}