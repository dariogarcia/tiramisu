#ifndef CNNFEATURES_H
#define CNNFEATURES_H

#include <string>
#include <vector>
#include "CNNScheme.hpp"

using std::string;
using std::vector;

struct Feature {
    float mean, stdDev;
};

class CNNFeatures {
  public:
    //Setters & Getters
    inline string getName() const {return name;}
    inline int getNumValues() const {return numValues;}
    inline void setName(string n) { name = n;}
    inline void setNumValues(int n) { name = n;}
    vector<vector<Feature> > features;
    
    void initialize(CNNScheme &scheme);

  protected:
    string name;
    int numValues;

}; //  CNNFeatures

#endif
