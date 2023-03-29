#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

int main(int argc, char** argv) {
  // cv::VideoCapture cap;

  // cap.open("udp://0.0.0.0:11111");

  // std::cout << "test" << std::endl;

  // while (true) {
  //   cv::Mat frame;

  //   if (cap.read(frame)) {
  //     cv::imshow("tello", frame);

  //     if (cv::waitKey(10) == 'c') {
  //       static int i = 0;
  //       std::string img_name;
  //       img_name += "calib_" + std::to_string(i) + ".jpg";
  //       cv::imwrite(img_name, frame);
  //       i++;
  //     }
  //   }
  // }

  std::vector<std::vector<cv::Point3f>> objectPointsArray;
  std::vector<std::vector<cv::Point2f>> imagePointsArray;
  std::vector<cv::Mat> images;

  cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);

  cv::Size image_size;

  for (int i = 0; i < 22; i++) {
    // 3D coordinates of chessboard points
    std::vector<cv::Point3f> objectPoints;
    for (int y = 0; y < 6; ++y) {
      for (int x = 0; x < 9; ++x) objectPoints.push_back(cv::Point3f(x * 0.025, y * 0.025, 0));
    }

    std::string image_name = "calib/calib_" + std::to_string(i) + ".jpg";
    cv::Mat image = cv::imread(image_name);

    if (image.empty()) {
      continue;
    }

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    std::vector<cv::Point2f> corners;

    bool pattern_was_found = cv::findChessboardCorners(gray_image, cv::Size(9, 6), corners);

    if (corners.empty()) {
      continue;
    }

    if (!pattern_was_found) {
      continue;
    }

    image_size = image.size();

    cv::cornerSubPix(gray_image, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

    cv::drawChessboardCorners(image, cv::Size(9, 6), corners, pattern_was_found);

    objectPointsArray.push_back(objectPoints);
    imagePointsArray.push_back(corners);
    images.push_back(image);

    // std::cout << image_name << std::endl;
    // cv::imshow("img", image);
    // if (cv::waitKey() > 0) {
    //   continue;
    // }
    // std::cout << image.size() << std::endl;
  }

  cv::Mat camera_matrix, dist_coeff, R, T;

  cv::calibrateCamera(objectPointsArray, imagePointsArray, image_size, camera_matrix, dist_coeff, R,
                      T);

  // std::vector<cv::Point2f> points = {cv::Point2f(960, 720)};
  // std::vector<cv::Point2f> undisorted_points;
  // cv::undistortPoints(points, undisorted_points, camera_matrix, dist_coeff);

  // std::cout << points << std::endl;
  // for (const auto& undis : undisorted_points) {
  //   std::cout << undis << std::endl;
  //   // std::cout << undis.x * 960 << " " << undis.y * 720 << std::endl;
  // }

  // for (const auto& image : images) {
  //   cv::Mat undistorted;
  //   cv::undistort(image, undistorted, camera_matrix, dist_coeff);

  //   cv::imshow("img", image);
  //   cv::imshow("undistort", undistorted);

  //   if (cv::waitKey() > 0) {
  //     continue;
  //   }
  // }

  std::cout << "Camera matrix : " << camera_matrix << std::endl;
  std::cout << "Dist coeff : " << dist_coeff << std::endl;
  // std::cout << R << std::endl;
  // std::cout << T << std::endl;

  return 0;
}