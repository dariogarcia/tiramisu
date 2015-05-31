#ifndef CNNLAYER_H
#define CNNLAYER_H

#include <string>
#include <map>

#include "CNNFeature.hpp"

using std::string;
using std::map;
using std::pair;

class CNNLayer {
  public:
    inline string getName() {return name;}
    inline map<int,CNNFeature> getFeatures() {return features; }
    inline void setName(string n) { name = n;}
    inline void setFeatures(map<int,CNNFeature> fs) { features = fs;}
    void addFeatures(const string path);
    void exploreCNNLayer();
    void computeLayerStatistics();
    pair<float,float> getFeatMeanAbsDev(int fId) {return pair<float,float> (features[fId].getMean(),features[fId].getStdDev());}

  protected:
    string name;
    map<int,CNNFeature> features;

}; //  CNNLayer

#endif
