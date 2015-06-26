#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <map>

#include "CNNLayer.hpp"

using std::string;
using std::map;

class Image {
  public:
    inline string getImageName() const {return imageName;}
    inline string getClassName() const {return className;}
    inline string getPath() const {return path;}
    inline const map<string,int>& getLayersIDX() const {return layers_IDX;}
    inline vector< vector<pair<int,float> > > getActivationsDyn() const {return activations;}
    inline const vector< vector<pair<int,float> > >& getActivationsConst() const {return activations;}
    inline vector< vector<pair<int,float> > >& getRelevantFeaturesDyn() {return relevantFeatures;}
    inline const vector< vector<pair<int,float> > >& getRelevantFeaturesConst() const {return relevantFeatures;}
    inline void setImageName(string n) {imageName=n;}
    inline void setClassName(string n) {className=n;}
    inline void setPath(string p) {path=p;}
    inline void setActivations(const vector< vector<pair<int,float> > > &a) {activations=a;}
    void addActivations(const string path, const string layerName);
    void exploreImage() ;
    void computeRelevantFeatures(const map<string,CNNLayer> &CNN);

  protected:
    string imageName;
    string className;
    string path;
    vector< vector<pair<int,float> > > activations;
    map<string,int> layers_IDX;
    vector< vector<pair<int,float> > > relevantFeatures;
}; //  Image

#endif
