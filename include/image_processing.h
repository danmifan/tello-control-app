#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <atomic>
#include <thread>
#include <math.h>

#include <opencv2/core.hpp>

#include "face_detection.h"
#include "tracker.h"
#include "aruco_detector.h"
#include "file_logger.h"

#define HFOV 50

class ImageProcessing {
 public:
  ImageProcessing(int width, int height, int channels);
  ~ImageProcessing();
  void start();
  unsigned char* getImage();
  void hover();

 private:
  unsigned char* image_ = nullptr;
  std::thread th_;
  std::atomic<bool> run_ = {false};

  // CV
  int width_;
  int height_;
  int channels_;
  Tracker tracker_;
  ArucoDetector aruco_detector_;
  FaceDetection face_detection_;

  FileLogger file_;

  bool hover_enabled_ = false;
  cv::Vec3d hover_pose_;
  bool first_aruco_pose_ = false;

  bool aruco_detector_enabled_ = false;
  bool face_detector_enabled_ = false;
};

#endif  // IMAGE_PROCESSING_H