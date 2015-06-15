#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

#include "../include/Image.hpp"

using std::vector;
using std::pair;
using std::stof;
using std::ifstream;
using std::istringstream;
using std::istream_iterator;
using std::back_inserter;

void Image::addActivations(string path, string layerName){
  ifstream infile(path.c_str());
  string line;
  if(infile.is_open()){
    map<int,Activation> &layerActivations = activations[layerName];
    map<int,float> values;
    while(getline(infile,line)){
      vector<std::string> strs;
      istringstream is(line);
      copy(istream_iterator<string>(is),istream_iterator<string>(),back_inserter<vector<string> >(strs));
      int key = stoi(strs[1]);
      float value = stof(strs[0]);
      Activation &featActivation = layerActivations[key];
      featActivation.setValue(value);
      featActivation.setLayerId(layerName);
      featActivation.setFeatureId(key);
    }
    infile.close();
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

void Image::computeRelevantFeatures(map<string,CNNLayer> &cnn){
  map<int,float> emptyMap;
  //For each layer
  for(map<string,CNNLayer>::iterator it = cnn.begin();it!=cnn.end();it++){
    map<int,CNNFeature> features = it->second.getFeatures();
    map<int,Activation> imageActivations = activations[it->first];
    //If its the first time we read a layer, add it
    if(relevantFeatures.find(it->first)==relevantFeatures.end())
      relevantFeatures.insert(pair<string,map<int,float> >(it->first,emptyMap));
    //For each feature in the layer
    for(map<int,CNNFeature>::iterator it2=features.begin(); it2!=features.end();it2++){
      //If the feature is strongly activated, store it
      if(imageActivations[it2->first].getValue() > it2->second.getActivationThreshold()){
        relevantFeatures[it->first].insert(pair<int,float> (it2->first,imageActivations[it2->first].getValue()));
      } 
    }
  }
}
