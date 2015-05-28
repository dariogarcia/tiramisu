#ifndef CNNFEATURE_H
#define CNNFEATURE_H

#include <string>
#include <vector>

#include "Image.hpp"

using std::string;
using std::vector;

class CNNFeature {
  public:
    inline int getId() {return id;}
    inline float getMean() {return mean; }
    inline float getDev() {return deviation; }
    inline vector<float> getValues() {return values; }
    inline void setId(int i) { id = i;}
    inline void addValue(float val) {values.push_back(val);} 
    void computeStatistics(vector<Image> im);

  protected:
    int id;
    float mean;
    float deviation;
    vector<float> values;
}; //  CNNFeature

#endif
