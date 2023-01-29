#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char** argv) {
  cv::VideoCapture cap;
  cap.open("udp://@0.0.0.0:11111");

  while (1) {
    cv::Mat frame;
    cap.read(frame);

    cv::imshow("Tello", frame);
    if (cv::waitKey(1) >= 0) {
      break;
    }
  }

  std::cout << "Bye" << std::endl;
  return 0;
}