#ifndef LOGGER_H
#define LOGGER_H

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#include <list>
#include <string>
#include <iostream>

class Log {
 public:
  static Log& get();
  void info(std::string msg);
  void warning(std::string msg);
  void error(std::string msg);
  std::list<std::string> getLogs();

 private:
  enum Type { INFO = 0, WARNING = 1, ERROR = 2 };

  Log();
  void message(Type type, std::string msg);

  std::list<std::string> logs_;
};

#endif  // LOGGER_H