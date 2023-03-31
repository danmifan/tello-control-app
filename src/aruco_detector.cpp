#include "aruco_detector.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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
}

void ArucoDetector::detect(cv::Mat frame) {
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
    cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.066, camera_matrix_, dist_, rvecs, tvecs);

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

      cv::putText(frame, ss.str(), org, cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255, 255));

      // std::cout << t << std::endl;
    }
  }
}