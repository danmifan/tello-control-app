#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include "data.h"

struct TrackData {
  Vec2i position;
  double width;
  double height;
};

class Tracker {
 public:
  Tracker();
  bool track(cv::Mat frame, TrackData& target);

 private:
  cv::Ptr<cv::legacy::TrackerMedianFlow> cv_tracker_;
  bool init_ = false;
  int rect_width_ = 160;
  int rect_height_ = 100;

  bool new_target_received_ = false;
  int target_x;
  int target_y;
};

#endif  // TRACKER_H