#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <GLFW/glfw3.h>

class MyWindow {
 public:
  MyWindow(int width, int height);
  int init();
  void update();
  void shutdown();

 private:
  GLFWwindow *window_;
  int width_;
  int height_;
};

#endif  // IMGUI_WINDOW_H