#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <GLFW/glfw3.h>
#include <list>

#include "aview.h"

class MyWindow {
 public:
  MyWindow(int width, int height, int framerate = 60);
  int init();
  void update();
  void shutdown();
  void addView(AView* widget);

#warning tmp
  unsigned char* getImage();
  GLuint* getTextures();

 private:
  GLFWwindow* window_;
  int width_;
  int height_;
  std::list<AView*> views_;
  unsigned char* image_data_;
  GLuint image_textures_[3];
  int framerate_;
};

#endif  // IMGUI_WINDOW_H