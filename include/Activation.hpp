#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <string>

using std::string;

class Activation {
  public:
    string getCNNLayerId() {return layerId;}
    int getFeatureId() {return featureId;}
    float getValue() {return value;}
    inline void setLayerId(string id) {layerId=id;}
    inline void setFeatureId(int id) {featureId=id;}
    inline void setValue(float v) {value=v;}

  protected:
    string layerId;
    int featureId;
    float value;
}; //  Activation

#endif
