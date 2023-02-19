#include "video_streaming.h"

#include <iostream>
#include <chrono>

VideoStreaming::~VideoStreaming() {
  if (run_) {
    run_ = false;
    th_.join();
  }
}

void VideoStreaming::start() {
  // std::cout << "OpenCV version : " << CV_VERSION << std::endl;
  // std::cout << "Major version : " << CV_MAJOR_VERSION << std::endl;
  // std::cout << "Minor version : " << CV_MINOR_VERSION << std::endl;
  // std::cout << "Subminor version : " << CV_SUBMINOR_VERSION << std::endl;

  if (!run_) {
    run_ = true;
  } else {
    return;
  }

  th_ = std::thread([&]() {
    while (!cap_.open("udp://0.0.0.0:11111", cv::CAP_FFMPEG) && run_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout << "Waiting for stream..." << std::endl;
    }

    while (run_) {
      cv::Mat frame;
      if (cap_.read(frame)) {
        if (!frame.empty()) {
#warning memory leak
          // frames_.push_back(frame);

          cv::Mat cvt_frame;

          // does converting introduce delay ?
          cv::cvtColor(frame, cvt_frame, cv::COLOR_BGR2RGB);
          std::cout << "Cvt : " << cvt_frame.size << std::endl;
          std::cout << cv::typeToString(cvt_frame.type()) << std::endl;

          memcpy(image_, cvt_frame.data, 960 * 720 * 3);
        }
        std::cout << frame.size << std::endl;
        std::cout << cv::typeToString(frame.type()) << std::endl;
      } else {
        std::cout << "no frame" << std::endl;
      }
    }
    cap_.release();
    std::cout << "Stream end" << std::endl;
  });
}

void VideoStreaming::stop() {
  if (run_) {
    run_ = false;
    th_.join();  // this might block the main thread
    std::cout << "Stream stopped" << std::endl;
  } else {
    std::cout << "Stream already stopped" << std::endl;
  }
}

void VideoStreaming::setImage(unsigned char *image) { image_ = image; }