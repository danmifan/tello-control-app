#include "video_streaming.h"

#include <iostream>
#include <chrono>
#include <opencv2/cudacodec.hpp>

#include "global.h"

std::deque<cv::Mat> frames_;

VideoStreaming::VideoStreaming(int width, int height, int channels) {
  image_ = (unsigned char*)malloc(width * height * channels);
  memset(image_, 0, width * height * channels);
}

VideoStreaming::~VideoStreaming() {
  if (run_) {
    run_ = false;
    th_.join();
  }

  delete image_;
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
    while (!cap_.open("udp://0.0.0.0:11111", cv::CAP_GSTREAMER) && run_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout << "Waiting for stream..." << std::endl;
    }

    while (run_) {
      auto t1 = std::chrono::high_resolution_clock::now();
      cv::Mat frame;
      if (cap_.read(frame)) {
        if (!frame.empty()) {
          cv::Mat cvt_frame;

          frames_.push_back(frame);

          // does converting introduce delay ?
          cv::cvtColor(frame, cvt_frame, cv::COLOR_BGR2RGB);

          memcpy(image_, cvt_frame.data, 960 * 720 * 3);
        }
      } else {
        std::cout << "no frame" << std::endl;
      }

      auto t2 = std::chrono::high_resolution_clock::now();
      int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

      int delta = 33 - duration_ms;

      if (delta > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delta));
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

unsigned char* VideoStreaming::getImage() { return image_; }