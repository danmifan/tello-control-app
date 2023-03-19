#include "logger.h"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

Log::Log() {}

Log &Log::get() {
  static Log instance;
  return instance;
}

void Log::message(Type type, std::string msg) {
  std::string color;
  std::string type_str;

  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);

  auto now = std::chrono::system_clock::now();

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

  std::ostringstream ss;
  ss << std::put_time(&tm, "%H:%M:%S");
  ss << " " << ms.count();
  std::string time_str = "[" + ss.str() + "] ";

  switch (type) {
    case INFO:
      color = GREEN;
      type_str = "[INFO]";
      break;

    case WARNING:
      color = YELLOW;
      type_str = "[WARNING]";
      break;

    case ERROR:
      color = RED;
      type_str = "[ERROR]";
      break;

    default:
      break;
  }

  std::string formated_msg = color + type_str + RESET + time_str + msg;
  std::string formated_msg_wo_color = type_str + time_str + msg;
  std::cout << formated_msg << std::endl;
  logs_.push_back(formated_msg_wo_color);
}

void Log::info(std::string msg) { message(Type::INFO, msg); }

void Log::warning(std::string msg) { message(Type::WARNING, msg); }

void Log::error(std::string msg) { message(Type::ERROR, msg); }

std::vector<std::string> Log::getLogs() { return logs_; }
