#ifndef UTIL_H
#define UTIL_H

#include "Image.hpp"

class Util {
  public:
    static void generate_random_string(char *s, const int len);
    static float euclideanDistanceActivations(const Image &img1, const Image &img2);
}; // UTIL

#endif
