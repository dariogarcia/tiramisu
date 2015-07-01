#ifndef CNNSCHEME_H
#define CNNSCHEME_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class CNNScheme {
  public:
    inline string getName() const {return name;}
    inline int getNumLayers() const {return numLayers;}
    inline void setName(string n) {name = n;}
    inline void setNumLayers(int n) {numLayers = n;}
    vector<string> layerIdx;
    vector<int> layerSize;
    //Print methods
    void printScheme();

  protected:
    string name;
    int numLayers;
}; //CNNScheme

#endif
