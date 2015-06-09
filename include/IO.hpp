#ifndef IO_H
#define IO_H

#include <string>
#include <map>

#include "Image.hpp"
#include "CNNLayer.hpp"

using std::string;
using std::map;
using std::pair;

class IO {
  public:
    static void loadDirectoryOfImages(string path, map<string,Image> &images, map<string,CNNLayer> &layers);

  protected:

}; //  IO

#endif
