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
  MainView(FlightControl* fc, VideoStreaming* vs, int image_width, int image_height,
           bool* joystick_enabled);
  void update() override;
  void showDroneVideoFeed();
  void showDroneStatus();
  void showCommands();
  void showConsole();
  void showOverlay();
  void setImage(unsigned char* image);
  void setFaceImage(unsigned char* image);
  void setImgProcImage(unsigned char* image);
  void setTextures(GLuint* textures);
  Event* getEvent();

 private:
  DroneState current_state_;
  FlightControl* fc_;
  VideoStreaming* vs_;
  unsigned char* image_;
  unsigned char* face_image_;
  unsigned char* imgproc_image_;
  GLuint* textures_;
  int image_width_;
  int image_height_;
  bool show_demo_ = false;
  Event* event_;
  Vec4i battery_color_;
  bool* joystick_enabled_;
};

#endif  // MAIN_VIEW_H