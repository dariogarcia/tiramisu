#ifndef UTIL_H
#define UTIL_H

#include "Image.hpp"
#include "ImageClass.hpp"
#include "CNNLayer.hpp"

class Util {
  public:
    static void generate_random_string(char *s, const int len);
    static float euclideanDistanceImageActivations(const map<string,CNNLayer> &layers, const Image &img1, const Image &img2);
    static float euclideanDistanceImageClass(const map<string,CNNLayer> &layers, const ImageClass &imgc1, const ImageClass &imgc2);
}; // UTIL

#endif
