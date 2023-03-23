#ifndef DATA_H
#define DATA_H

#include <sys/time.h>

struct Vec2i {
  int x = 0;
  int y = 0;
};

struct Vec3f {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};

struct Vec3i {
  int x = 0;
  int y = 0;
  int z = 0;
};

struct Vec4i {
  int x = 0;
  int y = 0;
  int z = 0;
  int w = 0;
};

struct Event {
  bool active = false;
  Vec2i data;
};

struct DroneState {
  Vec3i attitude;
  Vec3i velocity;
  int templ = 0;
  int temph = 0;
  int tof = 0;
  int h = 0;
  int bat = 0;
  float baro = 0.0f;
  int time = 0;
  Vec3f acceleration;
  struct timeval timestamp;
};

#endif  // DATA_H