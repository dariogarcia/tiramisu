#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <cmath>

#include "../include/Image.hpp"

using std::vector;
using std::pair;
using std::stof;
using std::ifstream;
using std::istringstream;
using std::istream_iterator;
using std::back_inserter;

void Image::normalizeActivations(){
  for(int i = 0; i<activations.size(); i++){
    vector<pair<int,float> > &currentLayer = activations[i];
    for(vector<pair<int,float> >::iterator it = currentLayer.begin(); it!=currentLayer.end(); it++){
      (*it).second = (*it).second/sqrt(unsquaredNorm);
    }
  } 

}

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
