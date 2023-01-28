#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <GLFW/glfw3.h>
#include <list>

#include "abstract_widget.h"

class MyWindow {
 public:
  MyWindow(int width, int height);
  int init();
  void update();
  void shutdown();
  void addWidget(AbstractWidget* widget);

 private:
  GLFWwindow* window_;
  int width_;
  int height_;
  std::list<AbstractWidget*> widgets_;
};

#endif  // IMGUI_WINDOW_H