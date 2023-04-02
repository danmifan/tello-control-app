#ifndef UTILS_H
#define UTILS_H

#include <algorithm>

inline int clamp(int n, int lower, int upper) { return std::max(lower, std::min(n, upper)); }

inline float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

#endif  // UTILS_H