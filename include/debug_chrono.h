#ifndef DEBUG_CHRONO_H
#define DEBUG_CHRONO_H

#include <iostream>

// clang-format off
#define CLOCK_MS(FUNCTION, name) auto t1 = std::chrono::high_resolution_clock::now(); \
FUNCTION; \
auto t2 = std::chrono::high_resolution_clock::now(); \
std::cout << "Function " << name << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
// clang-format on

#endif  // DEBUG_CHRONO_H