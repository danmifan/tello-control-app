#ifndef GLOBAL_H
#define GLOBAL_H

#include <deque>
#include "drone_status.h"
#include <opencv2/core.hpp>

#warning tmp, will be written properly later

#define VIDEO_WIDTH 960
#define VIDEO_HEIGHT 720

extern std::deque<DroneState> status_;
extern std::deque<cv::Mat> frames_;

#endif  // GLOBAL_H