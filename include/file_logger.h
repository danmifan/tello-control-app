#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <fstream>

class FileLogger {
 public:
  void write(int cmd_dx, int cmd_dy, int current_dx, int current_dy);

 private:
  std::ofstream of_;
};

#endif  // FILE_LOGGER_H