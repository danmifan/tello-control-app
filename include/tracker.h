#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/tracking.hpp>
#include "data.h"

struct TrackData {
  int dx;
  int dy;
  double width;
  double height;
};

class Tracker {
 public:
  Tracker();
  void setEvent(Event* event);
  bool track(cv::Mat frame, TrackData& target);

 private:
  cv::Ptr<cv::Tracker> cv_tracker_;
  bool init_ = false;
  int rect_width_ = 160;
  int rect_height_ = 100;
  Event* event_;
};

#endif  // TRACKER_H