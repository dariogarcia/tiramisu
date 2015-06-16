#ifndef IO_H
#define IO_H

#include <string>
#include <map>
#include <vector>

#include "Image.hpp"
#include "CNNLayer.hpp"

using std::string;
using std::map;
using std::vector;
using std::pair;

class IO {
  public:
    static void loadImagesAndLayersFromTXTFile(string path, map<string,Image> &images, map<string,CNNLayer> &layers);
    static void loadImagesFromTXTFile(string path, map<string,Image> &images);
    static void loadLayersFromTXTFile(string path, map<string,CNNLayer> &layers);
    static string writeImagesVerticesToTXTFile(const map<string,Image> &images);
    static string writeLayersVerticesToTXTFile(const map<string,CNNLayer> &layers);
    static string writeImagesAndLayersVerticesToTXTFile(const map<string,Image> &images, const map<string,CNNLayer> &layers);
    static string writeImagesAndLayersEdgesToTXTFile(const map<string,Image> &images, const map<string,CNNLayer> &layers);
    static vector<string> writeLayersToTXTFile(const map<string,CNNLayer> &layers);
    static void writeLayersToBinaryFile(string const filename, map<string,CNNLayer> const layers);
    static void loadLayersFromBinaryFile(string const filename, map<string,CNNLayer> &layers);

  protected:

}; //  IO

#endif
