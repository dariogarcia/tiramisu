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

//void Image::addActivations(string path, string layerName){
//  ifstream infile(path.c_str());
//  string line;
//  int NZV=0;
//  //Count non-zero elements
//  if(infile.is_open()){
//    while(getline(infile,line)){
//      vector<std::string> strs;
//      istringstream is(line);
//      copy(istream_iterator<string>(is),istream_iterator<string>(),back_inserter<vector<string> >(strs));
//      float value = stof(strs[0]);
//      if(value!=0) NZV++;
//    }
//    infile.close();
//  }
//  else {
//    printf("Image::addActivations:: Unable to open file\n");
//    return;
//  }
//  ifstream infile2(path.c_str());
//  if(infile2.is_open()){
//    //Set index for new layer, as the last
//    layers_IDX.insert(pair<string,int>(layerName,activations.size()));
//    vector<pair<int,float> > layerActivations (NZV);
//    while(getline(infile2,line)){
//      vector<std::string> strs;
//      istringstream is(line);
//      copy(istream_iterator<string>(is),istream_iterator<string>(),back_inserter<vector<string> >(strs));
//      int key = stoi(strs[1]);
//      float value = stof(strs[0]);
//      if(value!=0) layerActivations.push_back(pair<int,float>(key,value));
//    }
//    activations.push_back(layerActivations);
//    infile2.close();
//  }
//  else {
//    printf("Image::addActivations:: Unable to open file on a second attempt\n");
//    return;
//  }
//}
//
////Method to explore and print contents of a set of images
////Intended for validation of loading process
//void Image::exploreImage(){
//  printf("-Image with name '%s', path '%s', has '%u' layers with activations \n",imageName.c_str(),path.c_str(), (unsigned int)activations.size());
//  int activationCounter = 0;
//  for(map<string,int>::iterator it2 = layers_IDX.begin(); it2!=layers_IDX.end();it2++){
//    vector<pair<int,float> > &currentActivations = activations[it2->second];
//    printf("--Activations of layer num: '%u', name '%s', has '%u' values \n",
//      activationCounter, it2->first.c_str(), (unsigned int)currentActivations.size());
//    int valueCounter = 0;
//    for(vector<pair<int,float> >::iterator it3 = currentActivations.begin(); it3!=currentActivations.end();
//      it3++){
//      printf("---Value num: '%u' is '%f''\n",
//        valueCounter,it3->second);
//      valueCounter++;
//    }
//    activationCounter++;
//  }
//}
//
//void Image::computeRelevantFeatures(const map<string,CNNLayer> &cnn){
//  relevantFeatures.erase(relevantFeatures.begin(),relevantFeatures.end());
//  //For each layer
//  for(map<string,CNNLayer>::const_iterator it = cnn.begin();it!=cnn.end();it++){
//    vector<pair<int,float> > layerRelFeatures;
//    const map<int,CNNFeature> &features = it->second.getFeaturesConst();
//    vector<pair<int,float> > &imageActivations = activations[layers_IDX[it->first]];
//    ////If its the first time we read a layer, add it
//    //if(relevantFeatures.find(it->first)==relevantFeatures.end()){
//    //  map<int,float> emptyMap;
//    //  relevantFeatures.insert(pair<string,map<int,float> >(it->first,emptyMap));
//    //}
//    //For each activation in the layer of the image
//    for(vector<pair<int,float> >::iterator it2=imageActivations.begin(); it2!=imageActivations.end(); it2++){
//      //If the activation was larger than the threshold
//      if(it2->second > (features.find(it2->first))->second.getActivationThreshold()){
//        layerRelFeatures.push_back(pair<int,float>(it2->first,it2->second));
//      }
//    }
//    relevantFeatures.push_back(layerRelFeatures);
//  }
//}
