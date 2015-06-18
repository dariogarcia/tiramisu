#ifndef UTIL_H
#define UTIL_H

#include "Image.hpp"
#include "CNNLayer.hpp"

class Util {
  public:
    static void generate_random_string(char *s, const int len);
    static float euclideanDistanceActivations(const map<string,CNNLayer> &layers, const Image &img1, const Image &img2);
}; // UTIL

#endif
