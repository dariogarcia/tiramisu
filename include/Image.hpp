#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <map>

#include "Activation.hpp"

using std::string;
using std::map;

class Image {
  public:
    string getName() {return name;}
    string getPath() {return path;}
    map<string,map<int,Activation> > getActivations() {return activations;}
    inline void setName(string n) {name=n;}
    inline void setPath(string p) {path=p;}
    inline void setActivations(map<string,map<int,Activation> > a) {activations=a;}
    void addActivations(const string path, const string layerName);

  protected:
    string name;
    string path;
    map<string,map<int,Activation> > activations;
}; //  Image

#endif
