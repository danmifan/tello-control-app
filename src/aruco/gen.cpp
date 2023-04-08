#include <iostream>

#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>

int main(int /*argc*/, char** /*argv*/) {
  cv::Mat markerImage;
  cv::Ptr<cv::aruco::Dictionary> dictionary =
      cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  // cv::aruco::drawMarker(dictionary, 23, 200, markerImage, 1);
  // cv::imwrite("marker23.png", markerImage);

  cv::Ptr<cv::aruco::GridBoard> grid =
      cv::aruco::GridBoard::create(4, 6, 0.03, 0.007, dictionary, 120);
  grid->draw(cv::Size(1080, 1920), markerImage, 10, 1);

  cv::imwrite("marker_board.png", markerImage);

  return 0;
}