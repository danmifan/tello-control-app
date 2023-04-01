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
#include "pid_controller.h"
#include "file_logger.h"

#define HFOV 50

class ImageProcessing {
 public:
  ImageProcessing(FlightControl* fc);
  ~ImageProcessing();
  void start();
  void setEvent(Event* event);
  unsigned char* getImage();
  void setHoverPose();

 private:
  unsigned char* image_ = nullptr;
  std::thread th_;
  std::atomic<bool> run_ = {false};
  FlightControl* fc_;

  // CV
  Tracker tracker_;
  ArucoDetector aruco_detector_;
  FaceDetection face_detection_;

  // PID
  PIDController pid_;

  FileLogger file_;
};

#endif  // IMAGE_PROCESSING_H