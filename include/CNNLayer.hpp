#ifndef CNNLAYER_H
#define CNNLAYER_H

#include <string>
#include <map>

#include "CNNFeature.hpp"

using std::string;
using std::map;

class CNNLayer {
  public:
    inline string getName() {return name;}
    inline map<int,CNNFeature> getLayerFeatures() {return features; }
    inline void setName(string n) { name = n;}
    inline void setFeatures(map<int,CNNFeature> fs) { features = fs;}
    void addFeatures(const string path);

  protected:
    string name;
    map<int,CNNFeature> features;

}; //  CNNLayer

#endif
