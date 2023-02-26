#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <deque>
#include <thread>
#include <linux/joystick.h>

/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
inline int read_event(int fd, struct js_event *event) {
  ssize_t bytes;

  bytes = read(fd, event, sizeof(*event));

  if (bytes == sizeof(*event)) return 0;

  /* Error, could not read full event. */
  return -1;
}

/**
 * Current state of an axis.
 */
struct axis_state {
  short x, y;
};

/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
inline size_t get_axis_state(struct js_event *event, struct axis_state axes[3]) {
  size_t axis = event->number / 2;

  if (axis < 3) {
    if (event->number % 2 == 0)
      axes[axis].x = event->value;
    else
      axes[axis].y = event->value;
  }

  return axis;
}

struct JoystickData {
  int left_joystick_x = 0;
  int left_joystick_y = 0;
  int right_joystick_x = 0;
  int right_joystick_y = 0;
};

class Joystick {
 public:
  ~Joystick();
  void start();
  bool getData(JoystickData &data);

 private:
  const char *device_ = "/dev/input/js0";
  int js_;
  struct js_event event_;
  struct axis_state axes_[3] = {0};
  int max_axis_value_ = 32767;
  int dead_zone_ = 4000;
  std::deque<JoystickData> data_;
  std::thread th_;
};