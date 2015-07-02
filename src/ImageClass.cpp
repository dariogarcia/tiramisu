#include <iterator>
#include <algorithm>

#include "../include/ImageClass.hpp"
#include "../include/Util.hpp"

using std::find;

//Computes the mean activations based on a set of images which belong to the imageClass
//WARNING: Removes any previously stored meanActivations and image names
void ImageClass::computeMeanActivations(vector<pair<string,Image *> > &images, const CNNScheme &scheme){
  //printf("IN computeMeanActivations for %u images named:\n",images.size());
  //for(int i = 0;i<images.size();i++)printf("%s\n",images[i].first.c_str());
  meanActivations.clear();
  imageNames.clear();
  float staticNumImages =  (float)images.size();
  for(vector<pair<string,Image *> >::iterator it = images.begin(); it!=images.end();it++) imageNames.push_back(it->first);
  //For each layer
  for(int i = 0; i<scheme.getNumLayers();i++){
    float numImages = (float)images.size();
    vector<pair<int,float> > empty;
    meanActivations.push_back(empty);
    //printf("-Layer %u\n",i);
    //struct to save means of this layer
    vector<pair<int,float> > currentLayerMeans;
    //Struct to traverse all images concurrently, initialize at begin of each image values
    vector<pair<int,float> *> imagePointers;
    vector<pair<int,float> *> imagePointersEnds;
    for(int imageIndex = 0 ; imageIndex<images.size(); imageIndex++){
      imagePointers.push_back(&((images[imageIndex].second->activations[i]).front()));
      imagePointersEnds.push_back(&((images[imageIndex].second->activations[i]).back()));
      //printf("--Adding pointer\n");
    }
    //Current smallest index 
    int smallest = 0;
    //List of image indices with the smallest index initialize it with those having a value for '0'
    vector<int> indexSmallests;
    int count = 0;
    for(vector<pair<int,float> *>::iterator itP = imagePointers.begin(); itP!=imagePointers.end(); itP++){
      //TODO: What if image has no values? it breaks at this if?
      if((*itP)->first==0) indexSmallests.push_back(count);
      count++;
    }
    //printf("Going into loop, of %u only %u have value for 0\n", imagePointers.size(),indexSmallests.size());
    //TODO: THis can be probably optimized by removing pointers from imagePointers as they are finished
    //And iterating until the structure is empty.

    //While not all pointers are done, iterate
    while(true){
      //Combine values of smallest, advance pointers, store mean and clear list
      float combination = 0;
      for(int smCount = 0 ; smCount< indexSmallests.size(); smCount++){
        combination+=imagePointers[indexSmallests[smCount]]->second;
        if(imagePointers[indexSmallests[smCount]]==imagePointersEnds[indexSmallests[smCount]]){
          imagePointers.erase(imagePointers.begin()+indexSmallests[smCount]);
          imagePointersEnds.erase(imagePointersEnds.begin()+indexSmallests[smCount]);
          numImages--;
          //printf("Pointer of index %u cant be increased, removing\n",indexSmallests[smCount]);
        }
        else {
          imagePointers[indexSmallests[smCount]]++;
          //printf("Inceasing pointer of index %u \n",indexSmallests[smCount]);
        }
      }
      //printf("smallest feat %u combination %f indexSmallest.size %u \n",smallest,combination,indexSmallests.size());
      if(combination>0) meanActivations[i].push_back(pair<int,float>(smallest,combination/staticNumImages));
      indexSmallests.clear();
      //Check if we are done, if so exit
      bool done = true;
      for(int x = 0 ; x<numImages;x++) {
        done = done && imagePointers[x]==imagePointersEnds[x];
      }
      if(done) break;
      //Find  a first smallest candidate
      int newCount = 0;
      for(;newCount<numImages;newCount++){
        //Skip all finished elements
        if(imagePointers[newCount]==imagePointersEnds[newCount]){
          //printf("Pointer %u is done\n",newCount);
          continue;
        }
        //Found a first candidate
        else {
          indexSmallests.push_back(newCount); 
          smallest = imagePointers[newCount]->first;
          newCount++;
          break;
        }
      }
      //printf("First smallest candidate %u\n",smallest);
      //Given the first, find the smallest 
      for(;newCount<numImages;newCount++){
        //New image index for current smallest feature
        if(imagePointers[newCount]->first==smallest){
          //printf("Found another one\n");
          indexSmallests.push_back(newCount);
        }
        //If its smaller, reset and update
        else if(imagePointers[newCount]->first<smallest){
          indexSmallests.clear();
          indexSmallests.push_back(newCount);
          smallest = imagePointers[newCount]->first;
          //printf("Found a smaller one: %u on index %u\n",smallest,newCount);
        }
        newCount++;
      }
      //printf("Looping, smallest:%u for %u elems\n",smallest,indexSmallests.size());
    }
  //printf("-Done with Layer %u\n",i);
  }
  //printf("OUT computeMeanActivations \n");
  //for(int i = 0; i<scheme.getNumLayers();i++){
  //  printf("Found %u meanAct for layer idx %u\n",meanActivations[i].size(),i);
  //}
}


//Computes the mean activations based on a set of images which belong to the imageClass
//WARNING: Removes any previously stored meanActivations and image names
void ImageClass::computeMeanActivationsThreshold(vector<pair<string,Image *> > &images, const CNNScheme &scheme, const CNNFeatures &cnn){
  meanActivations.clear();
  imageNames.clear();
  float numImages = (float)images.size();
  for(vector<pair<string,Image *> >::iterator it = images.begin(); it!=images.end();it++) imageNames.push_back(it->first);
  //For each layer
  for(int i = 0 ; i < scheme.getNumLayers(); i++){
    int currentSize = scheme.layerSize[i];
    vector<pair<int,float> > currentMeans;
    //For each feature in the layer
    for(int j=0;j<currentSize;j++){
      float totalActivations=0.0;
      //Add the activation from each image
      for(vector<pair<string,Image *> >::iterator it = images.begin(); it!=images.end();it++){
         vector<pair<int,float> > &imageFeaturesForLayer = (it->second)->activations[i];
        for(vector<pair<int,float> >::iterator it2 = imageFeaturesForLayer.begin(); it2!=imageFeaturesForLayer.end(); it2++){
          if(it2->first==j) totalActivations+=it2->second;
          else if(it2->first>j)break;
        }
      }
      //Add value only if passes the threshold
      const Feature *f = &cnn.features[i][j];
      if(totalActivations/numImages > f->mean+2*f->stdDev){
        currentMeans.push_back(pair<int,float>(j,totalActivations/numImages));
      }
    }
    meanActivations.push_back(currentMeans);
  }
}


////Computes the mean activation from a set of images which are suposed to belong to this imageClass
////WARNING: Removes any previously stored meanActivations and image names
//void ImageClass::computeMeanActivations(const map<string,Image> &images, const map<string,CNNLayer> &CNN){
//  meanActivations.clear();
//  imageNames.clear();
//  int numImages = images.size();
//  for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++) imageNames.push_back(it3->first);
//  //For each layer
//  for(map<string,CNNLayer>::const_iterator it = CNN.begin(); it!=CNN.end(); it++){
//    string currentLayer = it->first;
//    const map<int,CNNFeature> &layerFeats = it->second.getFeaturesConst();
//    //If its the first time we read a layer, add it
//    if(meanActivations.find(currentLayer)==meanActivations.end()){
//      map<int,float> emptyMap;
//      meanActivations.insert(pair<string,map<int,float> >(currentLayer,emptyMap));
//    }
//    //For each feature in the layer
//    for(map<int,CNNFeature>::const_iterator it2=layerFeats.begin(); it2!=layerFeats.end();it2++){
//      int currentFeat = it2->first;
//      float totalActivations=0.0;
//      //Add the activation from each image
//      for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++){
//        int idx = it3->second.getLayersIDX().find(currentLayer)->second;
//        const vector<pair<int,float> > &activ = it3->second.getActivationsConst()[idx];
//        float val = 0.0;
//        for(vector<pair<int,float> >::const_iterator it_f = activ.begin();it_f!=activ.end();it_f++){
//          if(it_f->first<currentFeat)it_f++;
//          else if(it_f->first>currentFeat)break;
//          else val = it_f->second;
//        }
//        totalActivations+=val;
//      }
//      meanActivations[currentLayer][currentFeat] = totalActivations/(float)numImages;
//      //printf("processed feature %u\n",currentFeat);
//    }
//  }
//}
//
//
////Computes the mean activation of some images which belong to this imageClass.
////The list of images which belong are defined in the vector
////WARNING: Removes any previously stored meanActivations and image names
//void ImageClass::computeMeanActivations(vector<string> &imagesByClass, const map<string,Image> &images, const map<string,CNNLayer> &CNN){
//  meanActivations.clear();
//  imageNames.clear();
//  int numImages = imagesByClass.size();
//  //Store the names of the images belonging to the class (i.e., found in the vector)
//  for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++){
//    if(find(imagesByClass.begin(),imagesByClass.end(),it3->first)==imagesByClass.end()) continue;
//    imageNames.push_back(it3->first);
//  }
//  //For each layer
//  for(map<string,CNNLayer>::const_iterator it = CNN.begin(); it!=CNN.end(); it++){
//    string currentLayer = it->first;
//    const map<int,CNNFeature> &layerFeats = it->second.getFeaturesConst();
//    //If its the first time we read a layer, add it
//    if(meanActivations.find(currentLayer)==meanActivations.end()){
//      map<int,float> emptyMap;
//      meanActivations.insert(pair<string,map<int,float> >(currentLayer,emptyMap));
//    }
//    //For each feature in the layer
//    for(map<int,CNNFeature>::const_iterator it2=layerFeats.begin(); it2!=layerFeats.end();it2++){
//      int currentFeat = it2->first;
//      float totalActivations=0.0;
//      //Add the activation from each image
//      for(map<string,Image>::const_iterator it3 = images.begin(); it3!=images.end();it3++){
//        int idx = it3->second.getLayersIDX().find(currentLayer)->second;
//        const vector<pair<int,float> > &activ = it3->second.getActivationsConst()[idx];
//        float val = 0.0;
//        for(vector<pair<int,float> >::const_iterator it_f = activ.begin();it_f!=activ.end();it_f++){
//          if(it_f->first<currentFeat)it_f++;
//          else if(it_f->first>currentFeat)break;
//          else {
//            val = it_f->second;
//            break;
//          }
//        }
//        totalActivations+=val;
//        printf("adding activation of image %s which is %f and totals %f\n",it3->first,val,totalActivations):
//      }
//      if(totalActivations!=0.0) meanActivations[currentLayer][currentFeat] = totalActivations/(float)numImages;
//    }
//  }
//}

pair<ImageClass,float> ImageClass::findClosestClassByEucliDist(const vector<ImageClass> &iClassCandidates, const CNNScheme &scheme){
  const ImageClass &target = *this;
  if(iClassCandidates.size()<1){
    printf("ImageClass::findClosestClassByEucliDist::ERROR empty vector of candidates to compare. Returning self.\n");
    return pair<ImageClass,float>(target,0.0);
  }
  ImageClass closest;
  //Initialize closest distance to distance to first, unless its self, skip
  if(iClassCandidates.front().getName().compare(target.getName())==0){
    if(next(iClassCandidates.begin())==iClassCandidates.end()){
      printf("ImageClass::findClosestClassByEucliDist::ERROR one one class to compare and it is self. Returning self.\n");
      return pair<ImageClass,float>(target,0.0);
    }
    closest = *next(iClassCandidates.begin());
  }
  else closest = iClassCandidates.front();
  float closestDist = Util::euclideanDistanceImageClass(target, closest, scheme);
  //printf("%f %s --- %s\n",closestDist,target.getName().c_str(),closest.getName().c_str());
  for (vector<ImageClass>::const_iterator i = std::next(iClassCandidates.begin()); i != iClassCandidates.end(); i++){
    //Skip self comparation
    if((*i).getName().compare(target.getName())==0) continue;
    float currentDist = Util::euclideanDistanceImageClass(target, *i, scheme);
    if(currentDist<closestDist){
      closestDist=currentDist;
      closest = *i;
    }
    //printf("%f %s --- %s\n",currentDist,target.getName().c_str(),(*i).getName().c_str());
  }
  return pair<ImageClass,float>(closest,closestDist);
}

//pair<ImageClass,float> ImageClass::findClosestClassByEucliDist(const map<string,ImageClass> &iClassCandidates, const map<string,CNNLayer> &layers){
//  const ImageClass &target = *this;
//  if(iClassCandidates.size()<1){
//    printf("ImageClass::findClosestClassByEucliDist::ERROR empty vector of candidates to compare. Returning self.\n");
//    return pair<ImageClass,float>(target,0.0);
//  }
//  ImageClass closest;
//  //If the first is self, skip
//  if(iClassCandidates.begin()->second.getName().compare(target.getName())==0){
//    if(next(iClassCandidates.begin())==iClassCandidates.end()){
//      printf("ImageClass::findClosestClassByEucliDist::ERROR only one class to compare and it is self. Returning self.\n");
//      return pair<ImageClass,float>(target,0.0);
//    }
//    closest = next(iClassCandidates.begin())->second;
//  }
//  else closest = iClassCandidates.begin()->second;
//  float closestDist = Util::euclideanDistanceImageClass(layers, target, closest);
//  for (map<string,ImageClass>::const_iterator i = std::next(iClassCandidates.begin()); i != iClassCandidates.end(); i++){
//    //Skip self comparation
//    if(i->second.getName().compare(target.getName())==0) continue;
//    float currentDist = Util::euclideanDistanceImageClass(layers, target, i->second);
//    if(currentDist<closestDist){
//      printf("new closest for %s is %s at %f\n",target.getName().c_str(),i->first.c_str(),currentDist);
//      closestDist=currentDist;
//      closest = i->second;
//    }
//    printf("closest for %s is still %s with %f. %s was further at %f\n",target.getName().c_str(),closest.getName().c_str(),closestDist,i->first.c_str(),currentDist);
//  }
//  return pair<ImageClass,float>(closest,closestDist);
//}
//
//void ImageClass::printAccumulatedMeanAct(){
//  float total = 0;
//  for(map<string,map<int,float> >::iterator it = meanActivations.begin(); it!=meanActivations.end();it++){
//    map<int,float> &layer = it->second;
//    for(map<int,float>::iterator it2 = layer.begin(); it2!=layer.end(); it2++){
//      total+=it2->second;
//    }
//  }
//  printf("ImageClass::printAccumulatedMeanAct::ImageClass %s has an accumulated %f activations\n",name.c_str(),total);
//}
