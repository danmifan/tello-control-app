#include "face_detection.h"

#include <opencv2/imgproc.hpp>
#include <logger.h>

FaceDetection::FaceDetection() {
  net_ = cv::dnn::readNetFromCaffe("dnn/deploy.prototxt",
                                   "dnn/res10_300x300_ssd_iter_140000_fp16.caffemodel");

  net_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
  net_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
}

void FaceDetection::detect(const cv::Mat& frame) {
  // Face detect
  cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300));

  net_.setInput(blob);
  cv::Mat detect = net_.forward();
  cv::Mat detectionMat = cv::Mat(detect.size[2], detect.size[3], CV_32F, detect.ptr<float>());
  for (int i = 0; i < detectionMat.rows; i++) {
    float confidence = detectionMat.at<float>(i, 2);
    if (confidence > 0.7) {
      // std::cout << "Confidence : " << confidence << std::endl;
      int box_x = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
      int box_y = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
      int box_width = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols - box_x);
      int box_height = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows - box_y);
      cv::rectangle(frame, cv::Point(box_x, box_y),
                    cv::Point(box_x + box_width, box_y + box_height), cv::Scalar(255, 255, 255), 2);

      std::stringstream ss;
      ss << "Confidence : " << confidence;
      cv::putText(frame, ss.str(), cv::Point(box_x, box_y), cv::FONT_HERSHEY_PLAIN, 2,
                  cv::Scalar(255, 255, 255, 255));
    }
  }
}
