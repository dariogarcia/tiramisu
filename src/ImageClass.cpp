#include <iterator>

#include "../include/ImageClass.hpp"

//Computes the mean activation from a set of images which are suposed to belong to this imageClass
//WARNING: Removes any previously stored meanActivations and image names
void ImageClass::computeMeanActivations(const map<string,Image> &images, const map<string,CNNLayer> &CNN){
  meanActivations.clear();
  imageNames.clear();
  int numImages = images.size();
  for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++) imageNames.push_back(it3->first);
  //For each layer
  for(map<string,CNNLayer>::const_iterator it = CNN.begin(); it!=CNN.end(); it++){
    string currentLayer = it->first;
    const map<int,CNNFeature> &layerFeats = it->second.getFeaturesConst();
    printf("processing layer %s\n",currentLayer.c_str());
    //If its the first time we read a layer, add it
    if(meanActivations.find(currentLayer)==meanActivations.end()){
      map<int,float> emptyMap;
      meanActivations.insert(pair<string,map<int,float> >(currentLayer,emptyMap));
    }
    //For each feature in the layer
    for(map<int,CNNFeature>::const_iterator it2=layerFeats.begin(); it2!=layerFeats.end();it2++){
      int currentFeat = it2->first;
      float totalActivations;
      //Add the activation from each image
      for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++){
        const map<string,map<int,float> > &imageLayers = it3->second.getActivationsConst();
        const map<int,float> &imageFeats = imageLayers.find(currentLayer)->second;
        if(imageFeats!=imageLayers.end()->second){
          if(imageFeats.find(currentFeat)!=imageFeats.end()){
            //Add value
            totalActivations+=imageFeats.find(currentFeat)->second;
          }
        }
      }
      meanActivations[currentLayer][currentFeat] = totalActivations/(float)numImages;
      //printf("processed feature %u\n",currentFeat);
    }
  }
}
