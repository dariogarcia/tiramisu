#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

#include "../include/Image.hpp"

using std::vector;
using std::stof;
using std::ifstream;
using std::istringstream;
using std::istream_iterator;
using std::back_inserter;

void Image::addActivations(string path, string layerName){
  ifstream infile(path.c_str());
  string line;
  if(infile.is_open()){
    map<int,Activation> layerActivations = activations[layerName];
    map<int,float> values;
    while(getline(infile,line)){
      vector<std::string> strs;
      istringstream is(line);
      copy(istream_iterator<string>(is),istream_iterator<string>(),back_inserter<vector<string> >(strs));
      int key = stoi(strs[1]);
      float value = stof(strs[0]);
      Activation featActivation = layerActivations[key];
      featActivation.setValue(value);
      featActivation.setLayerId(layerName);
      featActivation.setFeatureId(key);
    }
    infile.close();
    activations[layerName] = layerActivations;
  }
  else printf("Image::addActivations:: Unable to open file\n");

}
