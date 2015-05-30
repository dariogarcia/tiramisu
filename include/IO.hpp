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
    static pair<map<string,Image>,map<string,CNNLayer> > loadDirectoryOfImages(string path);

  protected:

}; //  IO

#endif
