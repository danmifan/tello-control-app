#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include <opencv2/objdetect.hpp>

class FaceDetection {
 public:
  FaceDetection();
  void detect(const cv::Mat& frame);
  unsigned char* getFaceImage();

 private:
  cv::CascadeClassifier cascade_;
  unsigned char* face_image_;
};

#endif  // FACE_DETECTION_H