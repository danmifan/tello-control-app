#include "video_streaming.h"

#include <iostream>
#include <chrono>

#include "logger.h"
#include "global.h"
#include "event.h"

std::deque<cv::Mat> frames_;

VideoStreaming::VideoStreaming(int width, int height, int channels) {
  image_ = (unsigned char*)malloc(width * height * channels);
  memset(image_, 0, width * height * channels);

  gevent_dispatcher.appendListener("Screenshot", [&](const Event&) { take_screenshot_ = true; });
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

  // while (!cap_.open("udp://0.0.0.0:11111", cv::CAP_GSTREAMER) && run_) {

  th_ = std::thread([&]() {
    // std::string pipeline =
    //     "udpsrc port=11111 ! video/x-h264, stream-format=(string)byte-stream, width=(int)960, "
    //     "height=(int)720, framerate=(fraction)24/1, skip-first-bytes=2 ! queue ! decodebin ! "
    //     "videoconvert ! appsink";

    // std::string pipeline =
    //     "udpsrc port=11111 ! video/x-h264,width=960,height=720,framerate=24/1 ! h264parse "
    //     "!avdec_h264 !videoconvert ! appsink ";

    std::string pipeline =
        "udpsrc port=11111 ! video/x-h264,width=960,height=720,framerate=24/1 ! h264parse "
        "!avdec_h264 !queue ! videoconvert ! appsink ";

    while (!cap_.open(pipeline, cv::CAP_GSTREAMER) && run_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout << "Waiting for stream..." << std::endl;
    }

    while (run_) {
      auto t1 = std::chrono::high_resolution_clock::now();
      cv::Mat frame;
      if (cap_.read(frame)) {
        if (!frame.empty()) {
          if (take_screenshot_) {
            take_screenshot_ = false;
            static int count = 0;
            std::string filename = "screenshots/img_" + std::to_string(count) + ".jpg";
            cv::imwrite(filename, frame);
            Log::get().info("Screenshot taken : " + filename);
            count++;
          }

          cv::Mat cvt_frame;

          frames_.push_back(frame);

          cv::cvtColor(frame, cvt_frame, cv::COLOR_BGR2RGB);

          memcpy(image_, cvt_frame.data, 960 * 720 * 3);
        }
      } else {
        std::cout << "no frame" << std::endl;
      }

      auto t2 = std::chrono::high_resolution_clock::now();
      int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

      ThreadTimeEvent event("VideoStreaming", duration_ms);
      gevent_dispatcher.dispatch("ThreadTime", event);

      int delta = 33 - duration_ms;

      if (delta > 0) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(delta));
      }
    }
    cap_.release();
    Log::get().info("Stream end");
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