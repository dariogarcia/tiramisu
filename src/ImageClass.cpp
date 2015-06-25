#include <iterator>
#include <algorithm>

#include "../include/ImageClass.hpp"
#include "../include/Util.hpp"

using std::find;

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
    //If its the first time we read a layer, add it
    if(meanActivations.find(currentLayer)==meanActivations.end()){
      map<int,float> emptyMap;
      meanActivations.insert(pair<string,map<int,float> >(currentLayer,emptyMap));
    }
    //For each feature in the layer
    for(map<int,CNNFeature>::const_iterator it2=layerFeats.begin(); it2!=layerFeats.end();it2++){
      int currentFeat = it2->first;
      float totalActivations=0.0;
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


//Computes the mean activation of some images which belong to this imageClass.
//The list of images which belong are defined in the vector
//WARNING: Removes any previously stored meanActivations and image names
void ImageClass::computeMeanActivations(vector<string> &imagesByClass, const map<string,Image> &images, const map<string,CNNLayer> &CNN){
  meanActivations.clear();
  imageNames.clear();
  int numImages = imagesByClass.size();
  for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++){
    if(find(imagesByClass.begin(),imagesByClass.end(),it3->first)==imagesByClass.end()) continue;
    imageNames.push_back(it3->first);
  }
  //For each layer
  for(map<string,CNNLayer>::const_iterator it = CNN.begin(); it!=CNN.end(); it++){
    string currentLayer = it->first;
    const map<int,CNNFeature> &layerFeats = it->second.getFeaturesConst();
    //If its the first time we read a layer, add it
    if(meanActivations.find(currentLayer)==meanActivations.end()){
      map<int,float> emptyMap;
      meanActivations.insert(pair<string,map<int,float> >(currentLayer,emptyMap));
    }
    //For each feature in the layer
    for(map<int,CNNFeature>::const_iterator it2=layerFeats.begin(); it2!=layerFeats.end();it2++){
      int currentFeat = it2->first;
      float totalActivations=0.0;
      //Add the activation from each image
      for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++){
        if(find(imagesByClass.begin(),imagesByClass.end(),it3->first)==imagesByClass.end()) continue;
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
    }
  }
}

ImageClass ImageClass::findClosestClassByEucliDist(const vector<ImageClass> &iClassCandidates, const map<string,CNNLayer> &layers){
  const ImageClass &target = *this;
  if(iClassCandidates.size()<1){
    printf("ImageClass::findClosestClassByEucliDist::ERROR empty vector of candidates to compare. Returning self.\n");
    return target;
  }
  ImageClass closest;
  ///If the first is self, skip
  if(iClassCandidates.front().getName().compare(target.getName())==0){
    if(next(iClassCandidates.begin())==iClassCandidates.end()){
      printf("ImageClass::findClosestClassByEucliDist::ERROR one one class to compare and it is self. Returning self.\n");
      return target;
    }
    closest = *next(iClassCandidates.begin());
  }
  else closest = iClassCandidates.front();
  float closestDist = Util::euclideanDistanceImageClass(layers, target, closest);
  for (vector<ImageClass>::const_iterator i = std::next(iClassCandidates.begin()); i != iClassCandidates.end(); i++){
    //Skip self comparation
    if((*i).getName().compare(target.getName())==0) continue;
    float currentDist = Util::euclideanDistanceImageClass(layers, target, *i);
    if(currentDist<closestDist){
      closestDist=currentDist;
      closest = *i;
    }
  }
  return closest;
}

ImageClass ImageClass::findClosestClassByEucliDist(const map<string,ImageClass> &iClassCandidates, const map<string,CNNLayer> &layers){
  const ImageClass &target = *this;
  if(iClassCandidates.size()<1){
    printf("ImageClass::findClosestClassByEucliDist::ERROR empty vector of candidates to compare. Returning self.\n");
    return target;
  }
  ImageClass closest;
  //If the first is self, skip
  if(iClassCandidates.begin()->second.getName().compare(target.getName())==0){
    if(next(iClassCandidates.begin())==iClassCandidates.end()){
      printf("ImageClass::findClosestClassByEucliDist::ERROR one one class to compare and it is self. Returning self.\n");
      return target;
    }
    closest = next(iClassCandidates.begin())->second;
  }
  else closest = iClassCandidates.begin()->second;
  float closestDist = Util::euclideanDistanceImageClass(layers, target, closest);
  for (map<string,ImageClass>::const_iterator i = std::next(iClassCandidates.begin()); i != iClassCandidates.end(); i++){
    //Skip self comparation
    if(i->second.getName().compare(target.getName())==0) continue;
    float currentDist = Util::euclideanDistanceImageClass(layers, target, i->second);
    if(currentDist<closestDist){
      closestDist=currentDist;
      closest = i->second;
    }
  }
  return closest;
}
