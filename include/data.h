#ifndef DATA_H
#define DATA_H

#include <sys/time.h>

struct Vec3f {
  float x;
  float y;
  float z;
};

struct Vec3i {
  int x;
  int y;
  int z;
};

struct DroneState {
  Vec3i attitude;
  Vec3f velocity;
  int templ;
  int temph;
  int tof;
  int h;
  int bat;
  float baro;
  int time;
  Vec3f acceleration;
  struct timeval timestamp;
};

#endif  // DATA_H