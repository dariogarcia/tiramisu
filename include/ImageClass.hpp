#ifndef IMAGECLASS_H
#define IMAGECLASS_H

#include <string>
#include <vector>
#include <map>

#include "Image.hpp"
#include "CNNLayer.hpp"

using std::string;
using std::vector;
using std::map;

class ImageClass {
  public:
    inline string getName() const {return name;}
    inline const vector<string>& getImageNames() const {return imageNames;}
    inline const map<string,map<int,float> >& getMeanActivations() const {return meanActivations;}
    inline void setName(string n) {name=n;}
    inline void setImageNames(vector<string> n) {imageNames=n;}
    inline void addImageName(string n) {imageNames.push_back(n);}
    void computeMeanActivations(const map<string,Image> &images, const map<string,CNNLayer> &CNN);
    void computeMeanActivations(vector<string> &imageByClass, const map<string,Image> &images, const map<string,CNNLayer> &CNN);
    pair<ImageClass,float> findClosestClassByEucliDist(const vector<ImageClass> &iClassCandidates, const map<string,CNNLayer> &layers);
    pair<ImageClass,float> findClosestClassByEucliDist(const map<string,ImageClass> &iClassCandidates, const map<string,CNNLayer> &layers);

  protected:
    string name;
    vector<string> imageNames;
    map<string,map<int,float> > meanActivations;
}; //  ImageClass

#endif
