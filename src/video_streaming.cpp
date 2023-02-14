#include "video_streaming.h"

#include <iostream>
#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

VideoStreaming::~VideoStreaming() {
  run_ = false;
  th_.join();
}

void VideoStreaming::init() {
  th_ = std::thread([&]() {
    cv::VideoCapture cap;

    // if (cap.open("udp://0.0.0.0:11111", cv::CAP_GSTREAMER)) {
    //   std::cout << "Open" << std::endl;
    // } else {
    //   std::cout << "Open failed" << std::endl;
    // }

    while (!cap.open("udp://0.0.0.0:11111", cv::CAP_GSTREAMER) && run_) {
      std::cout << "what" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    while (run_) {
      cv::Mat frame;
      if (cap.read(frame)) {
        // push into queue ?
        // std::cout << frame.size << std::endl;
        // std::cout << cv::typeToString(frame.type()) << std::endl;
      } else {
        // std::cout << "no frame" << std::endl;
      }

      // if (cv::waitKey(1) >= 0) {
      //   break;
      // }
    }

    std::cout << "end of loop" << std::endl;
  });
}