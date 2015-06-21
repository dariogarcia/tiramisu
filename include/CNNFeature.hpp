#ifndef CNNFEATURE_H
#define CNNFEATURE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class CNNFeature {
  public:
    inline int getId() const {return id;}
    inline int getNumValues() const {return numValues;}
    inline float getMean() const {return mean; }
    inline float getStdDev() const {return standardDev; }
    inline float getAbsDev() const {return absoluteDev; }
    inline float getActivationThreshold() const {return activationThreshold; }
    inline vector<float> getValues() {return values; }
    inline void setId(int i) { id = i;}
    inline void setNumValues(int n) { numValues = n;}
    inline void increaseNumValues() { numValues++;}
    inline void setMean(float m) {mean = m; }
    inline void setStdDev(float d) {standardDev = d; }
    inline void setAbsDev(float d) {absoluteDev = d; }
    inline void setActivationThreshold(float at) {activationThreshold = at; }
    inline void addValue(float val) {values.push_back(val);} 
    void computeMean();
    void computeAbsDev();
    void computeStdDev();
    void computeActivationThreshold();
    void computeStatistics();

  protected:
    int id;
    int numValues;
    float mean;
    float standardDev;
    float absoluteDev;
    float activationThreshold;
    vector<float> values;
}; //  CNNFeature

#endif
