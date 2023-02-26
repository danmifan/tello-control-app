#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "data.h"
#include "window/abstract_widget.h"
#include "flight_control.h"
#include "video_streaming.h"
#include <GL/gl.h>

#include "global.h"

class MainView : public AbstractWidget {
 public:
  MainView(FlightControl* fc, VideoStreaming* vs, int image_width, int image_height)
      : fc_(fc), vs_(vs), image_width_(image_width), image_height_(image_height) {}
  void update() override;
  void showDroneVideoFeed();
  void showDroneStatus();
  void showCommands();
  void showConsole();
  void setImage(unsigned char* image);
  void setFaceImage(unsigned char* face_image);
  void setTextures(GLuint* textures);

 private:
  DroneState current_state_;
  FlightControl* fc_;
  VideoStreaming* vs_;
  unsigned char* image_;
  unsigned char* face_image_;
  GLuint* textures_;
  int image_width_;
  int image_height_;
  bool show_demo_ = false;
};

#endif  // MAIN_VIEW_H