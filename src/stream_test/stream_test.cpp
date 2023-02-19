#include <iostream>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

int main() {
  cv::VideoCapture cap;
  while (!cap.open("udp://0.0.0.0:11111", cv::CAP_FFMPEG)) {
    std::cout << "Waiting for stream..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  while (cv::waitKey(10) == 0) {
    cv::Mat frame;
    if (cap.read(frame)) {
      // push into queue ? check size > 0 before pushing to queue
      std::cout << frame.size << std::endl;
      // std::cout << cv::typeToString(frame.type()) << std::endl;

      cv::imshow("Tello", frame);
    } else {
      std::cout << "no frame" << std::endl;
    }
  }

  cap.release();

  std::cout << "Stream end" << std::endl;
  return 0;
}