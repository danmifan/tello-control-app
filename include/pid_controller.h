#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <chrono>

class PIDController {
 public:
  PIDController(float kp, float ki, float kd);
  float correct(float current, float goal);

 private:
  float kp_;
  float ki_;
  float kd_;
  float integral_ = 0.0f;
  float previous_error_ = 0.0f;
  float min = -100;
  float max = 100;
  // std::chrono::high_resolution_clock::time_point t1_;
};

#endif  // PID_CONTROLLER_H