#include <fstream>
#include <sstream>
#include <iterator>
#include "../include/CNNFeatures.hpp"

//Initialize the vectors of a CNNFeature according to the sizes of a CNNScheme
void CNNFeatures::initialize(CNNScheme &scheme){
  this->features.resize(scheme.getNumLayers());
  int layerCount = 0;
  Feature f;
  f.mean=0;
  f.stdDev=0;
  int counter = 0;
  for(vector<string>::iterator it = scheme.layerIdx.begin(); it!= scheme.layerIdx.end(); it++){
    vector<Feature> current  (scheme.layerSize[counter],f);
    this->features[layerCount].resize(scheme.layerSize[counter]);
    this->features[layerCount] = current;
  }
}
