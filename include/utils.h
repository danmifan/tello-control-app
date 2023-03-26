#ifndef UTILS_H
#define UTILS_H

#include <algorithm>

int clamp(int n, int lower, int upper) { return std::max(lower, std::min(n, upper)); }

#endif  // UTILS_H