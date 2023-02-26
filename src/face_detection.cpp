#include "face_detection.h"

#include <opencv2/imgproc.hpp>
#include <logger.h>

#include "debug_chrono.h"

FaceDetection::FaceDetection() {
  cv::String filename = cv::samples::findFile("data/haarcascade_frontalface_alt.xml");
  if (!cascade_.load(filename)) {
    Log::get().error("Could not load cascade samples");
    return;
  }

  face_image_ = (unsigned char*)malloc(960 * 720 * 3);
}

void FaceDetection::detect(const cv::Mat& frame) {
  cv::Mat gray;
  cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
  cv::equalizeHist(gray, gray);

  std::vector<cv::Rect> faces;
  CLOCK_MS(cascade_.detectMultiScale(gray, faces), "cascade");

  Log::get().info("Faces : " + std::to_string(faces.size()));
  for (const auto& face : faces) {
    cv::rectangle(frame, face, cv::Scalar(0, 255, 0));
  }

  cv::Mat tmp;
  cv::cvtColor(frame, tmp, cv::COLOR_BGR2RGB);
  memcpy(face_image_, tmp.data, 960 * 720 * 3);
}

unsigned char* FaceDetection::getFaceImage() { return face_image_; }