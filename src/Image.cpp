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
      layerActivations[key] = featActivation;
    }
    infile.close();
    activations[layerName] = layerActivations;
  }
  else printf("Image::addActivations:: Unable to open file\n");
}

//Method to explore and print contents of a set of images
//Intended for validation of loading process
void Image::exploreImage(){
  printf("-Image with name '%s', path '%s', has '%lu' activations \n",name.c_str(),path.c_str(),activations.size());
  int activationCounter = 0;
  for(map<string,map<int,Activation> >::iterator it2 = activations.begin();it2!=activations.end(); it2++){
    map<int,Activation> currentActivation = it2->second;
    printf("--Activation num: '%u', name '%s', has '%lu' values \n",
      activationCounter, it2->first.c_str(), currentActivation.size());
    int valueCounter = 0;
    for(map<int,Activation>::iterator it3 = currentActivation.begin(); it3!=currentActivation.end();
      it3++){
      printf("---Value num: '%u', is '%f' for layer '%s' and feature '%u'\n",
        valueCounter,it3->second.getValue(),it3->second.getCNNLayerId().c_str(),it3->second.getFeatureId());
      valueCounter++;
    }
    activationCounter++;
  }
}
