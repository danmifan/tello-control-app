#include <iostream>
#include <System.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv) {
  ORB_SLAM3::System slam("/home/vincent/workspace/ORB_SLAM3/Vocabulary/ORBvoc.txt",
                         "cfg/calib.yaml", ORB_SLAM3::System::MONOCULAR, true);

  cv::VideoCapture cap_;

  std::string pipeline =
      "udpsrc port=11111 ! video/x-h264,width=960,height=720,framerate=24/1 ! h264parse "
      "!avdec_h264 !queue ! videoconvert ! appsink ";

  while (!cap_.open(pipeline, cv::CAP_GSTREAMER)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Waiting for stream..." << std::endl;
  }

  while (1) {
    cv::Mat frame;
    if (cap_.read(frame)) {
      if (!frame.empty()) {
        double ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::high_resolution_clock::now().time_since_epoch())
                        .count();
        slam.TrackMonocular(frame, ts);
      } else {
        std::cout << "empty" << std::endl;
      }
    } else {
      std::cout << "No frame" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
  }

  return 0;
}