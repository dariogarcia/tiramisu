#ifndef UTIL_H
#define UTIL_H

#include "Image.hpp"
#include "ImageClass.hpp"

class Util {
  public:
    static void generate_random_string(char *s, const int len);
    //static float euclideanDistanceImageActivations(const map<string,CNNLayer> &layers, const Image &img1, const Image &img2);
    static double euclideanDistanceImageClass(const ImageClass &imgc1, const ImageClass &imgc2, const CNNScheme &scheme);
    static double cosineDistanceImageClass(const ImageClass &imgc1, const ImageClass &imgc2, const CNNScheme &scheme);
    static void computeImageClasses(vector<Image> &images, const CNNScheme &scheme,  vector<ImageClass> &imageClasses, int meanType);
    static void substractImageClass(ImageClass &imgc1, ImageClass &imgc2, ImageClass &result, const CNNScheme &scheme);
    static void addImageClass(ImageClass &imgc1, ImageClass &imgc2, ImageClass &result, const CNNScheme &scheme);

}; // UTIL

#endif
