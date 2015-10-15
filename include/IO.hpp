#ifndef IO_H
#define IO_H

#include <string>
#include <vector>

#include "CNNScheme.hpp"
#include "CNNFeatures.hpp"
#include "Image.hpp"
#include "ImageClass.hpp"

using std::string;
using std::vector;
using std::pair;

class IO {
  public:
    static void loadSchemeFromTXTFile(string path, CNNScheme &scheme, string pattern);
    static void loadSchemeFromCFGFile(string path, CNNScheme &scheme, string pattern);
    static void loadFeaturesFromTXTFile(string path, CNNFeatures &features, CNNScheme &scheme);
    static void loadImagesFromIVFFile(string path, vector<Image> &images, CNNScheme &scheme);
    //static void loadImagesAndLayersFromTXTFile(string path, map<string,Image> &images, map<string,CNNLayer> &layers);
    static void loadImagesFromTXTFile(string path, vector<Image> &images, CNNScheme &scheme, string pattern);
    static void readAndSetImageClasses(string path, vector<Image> &images);
    //static string writeImagesVerticesToTXTFile(string filename, const map<string,Image> &images);
    //static string writeLayersVerticesToTXTFile(string filename, const map<string,CNNLayer> &layers);
    //static string writeImagesAndLayersVerticesToTXTFile(string filename, const map<string,Image> &images, const map<string,CNNLayer> &layers);
    //static string writeImagesAndLayersEdgesToTXTFile(string filename, const map<string,Image> &images, const map<string,CNNLayer> &layers);
    //static vector<string> writeLayersToTXTFile(string filename, const map<string,CNNLayer> &layers);
    //static void writeLayersToBinaryFile(string const filename, map<string,CNNLayer> const &layers);
    //static void loadLayersFromBinaryFile(string const filename, map<string,CNNLayer> &layers);
    //static void writeImageClassToBinaryFile(string const filename, ImageClass const &imageC);
    static void writeImageClassToIVF(string const filename, ImageClass const &imageC, const CNNScheme &scheme);
    static void writeImagesOfClassToIVF(vector<Image> const &images, string const className, const CNNScheme &scheme);


  protected:

}; //  IO

#endif
