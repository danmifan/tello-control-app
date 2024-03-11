#ifndef GLOBAL_H
#define GLOBAL_H

#include <deque>
#include <map>
#include <opencv2/core.hpp>

#warning wip
extern std::deque<cv::Mat> frames_;
extern std::vector<int> pid_values_;

extern std::vector<int> x_cmds;
extern std::vector<int> y_cmds;
extern std::vector<int> z_cmds;
extern std::vector<int> yaw_cmds;

#endif  // GLOBAL_H