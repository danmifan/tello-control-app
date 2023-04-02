#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include <opencv2/dnn.hpp>
#include <opencv2/objdetect/face.hpp>

class FaceDetection {
 public:
  FaceDetection();
  void detect(const cv::Mat& frame);

 private:
  // cv::dnn::dnn4_v20191202::Net net_;
  cv::dnn::dnn4_v20220524::Net net_;
};

#endif  // FACE_DETECTION_H