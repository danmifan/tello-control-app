#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <vector>
#include "window/aview.h"
#include <GL/gl.h>
#include "data.h"

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

 private:
  unsigned char* image_;
  unsigned char* imgproc_image_;
  GLuint* textures_;
  int image_width_;
  int image_height_;
  bool show_demo_ = false;
  bool aruco_detector_enabled_ = false;
  bool face_detector_enabled_ = false;

  std::vector<ArucoMarker> aruco_status_;
};

#endif  // MAIN_VIEW_H