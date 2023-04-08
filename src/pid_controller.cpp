#include "pid_controller.h"

#include "utils.h"
#include "logger.h"

PIDController::PIDController(float kp, float ki, float kd) : kp_(kp), ki_(ki), kd_(kd) {}

float PIDController::correct(float current, float goal) {
  // auto t2 = std::chrono::high_resolution_clock::now();
  // int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1_).count();
  // float dt = duration_ms * 1e-3;

  float dt = 0.033;

  // Log::get().info("dt : " + std::to_string(dt));
  // Log::get().info("Duration : " + std::to_string(duration_ms));

  float error = goal - current;
  // Proportional
  float p = kp_ * error;

  // Integral
  integral_ += error * dt;
  float i = ki_ * integral_;

  // Derivative
  float derivative = (error - previous_error_) / dt;
  float d = kd_ * derivative;

  float pid = p + i + d;

  previous_error_ = error;

  // Log::get().info("P : " + std::to_string(p) + " " + "I : " + std::to_string(i) + " " +
  //                 "D : " + std::to_string(d));
  // Log::get().info(" P + I + D : " + std::to_string(pid));

  pid = clamp(pid, min, max);

  // t1_ = std::chrono::high_resolution_clock::now();

  return pid;
}