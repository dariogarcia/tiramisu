#ifndef CNNFEATURE_H
#define CNNFEATURE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class CNNFeature {
  public:
    inline int getId() {return id;}
    inline float getMean() {return mean; }
    inline float getStdDev() {return standardDev; }
    inline float getAbsDev() {return absoluteDev; }
    inline float getActivationThreshold() {return activationThreshold; }
    inline vector<float> getValues() {return values; }
    inline void setId(int i) { id = i;}
    inline void setMean(float m) {mean = m; }
    inline void setStdDev(float d) {standardDev = d; }
    inline void setAbsDev(float d) {absoluteDev = d; }
    inline void addValue(float val) {values.push_back(val);} 
    void computeMean();
    void computeAbsDev();
    void computeStdDev();
    void computeActivationThreshold();
    void computeStatistics();

  protected:
    int id;
    float mean;
    float standardDev;
    float absoluteDev;
    float activationThreshold;
    vector<float> values;
}; //  CNNFeature

#endif
