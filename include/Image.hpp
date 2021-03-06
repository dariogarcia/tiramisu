#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <map>

#include "CNNScheme.hpp"
#include "CNNFeatures.hpp"

using std::string;
using std::map;
using std::pair;

class Image {
  public:
    inline string getImageName() const {return imageName;}
    inline string getClassName() const {return className;}
    inline string getPath() const {return path;}
    //inline vector<vector<int> >& getRelevantFeaturesDyn() {return relevantFeatures;}
    //inline const vector<vector<int> >& getRelevantFeaturesConst() const {return relevantFeatures;}
    inline void setImageName(string n) {imageName=n;}
    inline void setClassName(string n) {className=n;}
    inline void setPath(string p) {path=p;}
    void normalizeActivations(int normType);
    vector< vector<pair<int,float> > > activations;
    vector<double> normByLayer;
    double unsquaredNorm;
    //void addActivations(const string path, const string layerName);
    //void exploreImage() ;
    //void computeRelevantFeatures(const CNNScheme &scheme, const CNNFeatures &cnnfeatures);

  private:
    string imageName;
    string className;
    string path;
    //vector<vector<int> > relevantFeatures;
}; //  Image

#endif
