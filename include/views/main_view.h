#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "data.h"
#include "window/aview.h"
#include <GL/gl.h>
#include "image_processing.h"

#include "global.h"

class MainView : public AView {
 public:
  MainView(int image_width, int image_height);
  void update() override;
  void showDroneVideoFeed();
  void showConsole();
  void showOverlay();
  void setImage(unsigned char* image);
  void setImgProcImage(unsigned char* image);
  void setTextures(GLuint* textures);
  Event* getEvent();

 private:
  unsigned char* image_;
  unsigned char* imgproc_image_;
  GLuint* textures_;
  int image_width_;
  int image_height_;
  bool show_demo_ = false;
  Event* event_;
  bool aruco_detector_enabled_ = false;
  bool face_detector_enabled_ = false;
};

#endif  // MAIN_VIEW_H