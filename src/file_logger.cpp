#include "file_logger.h"

#include <iomanip>
#include <ctime>

void FileLogger::write(int cmd_dx, int cmd_dy, int current_dx, int current_dy) {
  if (!of_.is_open()) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;

    ss << std::put_time(&tm, "%m-%d-%H-%M-%S") << ".csv";
    of_.open(ss.str());
    of_ << "cmd_dx;cmd_dy;current_dx;current_dy;" << std::endl;
  }

  of_ << std::to_string(cmd_dx) << ";" << std::to_string(cmd_dy) << ";"
      << std::to_string(current_dx) << ";" << std::to_string(current_dy) << ";" << std::endl;
}