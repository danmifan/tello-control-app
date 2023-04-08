#ifndef GLOBAL_H
#define GLOBAL_H

#include <deque>
#include <map>
#include "drone_status.h"
#include <opencv2/core.hpp>

#warning tmp, will be written properly later

#define VIDEO_WIDTH 960
#define VIDEO_HEIGHT 720

extern std::map<std::string, int> thread_time_;
extern std::deque<DroneState> status_;
extern std::deque<cv::Mat> frames_;
extern std::vector<int> pid_values_;

extern std::vector<int> x_cmds;
extern std::vector<int> y_cmds;
extern std::vector<int> z_cmds;
extern std::vector<int> yaw_cmds;

#endif  // GLOBAL_H