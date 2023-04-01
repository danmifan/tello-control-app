#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>

#include <thread>

int main(int argc, char** argv) {
  // cv::Mat frame = cv::imread("dnn/test.jpg");
  cv::Mat frame = cv::imread("myframe.jpg");

  auto net = cv::dnn::readNetFromCaffe("dnn/deploy.prototxt",
                                       "dnn/res10_300x300_ssd_iter_140000_fp16.caffemodel");

  cv::VideoCapture cap_;

  while (!cap_.open("udp://0.0.0.0:11111", cv::CAP_GSTREAMER)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Waiting for stream..." << std::endl;
  }

  while (1) {
    cv::Mat frame;
    if (cap_.read(frame)) {
      if (!frame.empty()) {
        // std::cout << "Cvt : " << cvt_frame.size << std::endl;
        // std::cout << cv::typeToString(cvt_frame.type()) << std::endl;
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300));

        net.setInput(blob);
        cv::Mat detect = net.forward();
        cv::Mat detectionMat = cv::Mat(detect.size[2], detect.size[3], CV_32F, detect.ptr<float>());
        for (int i = 0; i < detectionMat.rows; i++) {
          int class_id = detectionMat.at<float>(i, 1);
          float confidence = detectionMat.at<float>(i, 2);
          if (confidence > 0.7) {
            std::cout << "Confidence : " << confidence << std::endl;
            int box_x = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int box_y = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int box_width = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols - box_x);
            int box_height = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows - box_y);
            cv::rectangle(frame, cv::Point(box_x, box_y),
                          cv::Point(box_x + box_width, box_y + box_height),
                          cv::Scalar(255, 255, 255), 2);
          }
        }
      } else {
        std::cout << "no frame" << std::endl;
      }
    }
  }

  // while (1) {
  //   cv::imshow("osef", frame);
  //   if (cv::waitKey() > 0) {
  //     break;
  //   }
  // }
  return 0;
}