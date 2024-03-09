#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <atomic>
#include <thread>
#include <math.h>

#include <opencv2/core.hpp>

#include "face_detection.h"
#include "flight_control.h"
#include "tracker.h"
#include "aruco_detector.h"
#include "file_logger.h"

#define HFOV 50

class ImageProcessing {
 public:
  ImageProcessing(FlightControl* fc);
  ~ImageProcessing();
  void start();
  void setEvent(Event* event);
  unsigned char* getImage();
  void hover();

  void enableArucoDetector(bool enable);
  void enableFaceDetector(bool enable);

 private:
  unsigned char* image_ = nullptr;
  std::thread th_;
  std::atomic<bool> run_ = {false};
  FlightControl* fc_;

  // CV
  Tracker tracker_;
  ArucoDetector aruco_detector_;
  FaceDetection face_detection_;

  FileLogger file_;

  bool hover_enabled_ = false;
  cv::Vec3d hover_pose_;
  bool first_aruco_pose_ = false;

  bool aruco_detector_enabled_;
  bool face_detector_enabled_;
};

#endif  // IMAGE_PROCESSING_H