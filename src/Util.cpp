#include <cstdlib>
#include <ctime>
#include <cmath>

#include "../include/Util.hpp"
#include "../include/Image.hpp"

using std::fabs;

void Util::generate_random_string(char *s, const int len) {
  srand(time(0));
  static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < len; ++i) {
    s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
  }
  s[len] = 0;
}

//float Util::euclideanDistanceImageActivations(const map<string,CNNLayer> &layers, const Image &img1, const Image &img2){
//  float distance=0.0; 
//  //For each layer
//  for(map<string,CNNLayer>::const_iterator l_it=layers.begin(); l_it!=layers.end(); l_it++){
//    //If layer does not exist in one image, error by now. TODO: Avoid error, assume al features are 0
//    if(img1.getLayersIDX().find(l_it->first)==img1.getLayersIDX().end()||
//    img2.getLayersIDX().find(l_it->first)==(img2.getLayersIDX().end())){    
//      printf("Util::euclideanDistanceActivations::ERROR::Image/s do not contain layer\n");
//      return 0.0;
//    }
//    const map<string,int>& layersIDX = img1.getLayersIDX();
//    const vector<pair<int,float> > &act1 = img1.getActivationsConst()[layersIDX.find(l_it->first)->second];
//    const vector<pair<int,float> > &act2 = img2.getActivationsConst()[layersIDX.find(l_it->first)->second];
//    //const map<int,float> &act1 = img1.getActivationsConst().find(l_it->first)->second;
//    //const map<int,float> &act2 = img2.getActivationsConst().find(l_it->first)->second;
//    //for(map<int,CNNFeature>::const_iterator f_it= l_it->second.getFeaturesConst().begin(); f_it!= l_it->second.getFeaturesConst().end(); f_it++){
//    vector<pair<int,float> >::const_iterator it1 = act1.begin();
//    vector<pair<int,float> >::const_iterator it2 = act2.begin();
//    while(it1!=act1.end() && it2!=act2.end()){
//      if(it1->first<it2->first){
//        distance+=(it1->second*it1->second);
//        it1++;
//      }
//      else if(it2->first<it1->first){
//        distance+=(it2->second*it2->second);
//        it2++;
//      }
//      else{
//        distance+=(it1->second-it2->second)*(it1->second-it2->second);
//        it1++;
//        it2++;
//      }
//    }
//  }
//  return sqrt(distance);
//}

double Util::euclideanDistanceImageClass(const ImageClass &imgc1, const ImageClass &imgc2, const CNNScheme &scheme){
  double distance=0.0;
  //printf("Computing distance between %s and %s\n",imgc1.getName().c_str(),imgc2.getName().c_str());
  if(scheme.getNumLayers()!= imgc1.meanActivations.size() || scheme.getNumLayers()!= imgc2.meanActivations.size()){
    printf("Util::euclideanDistanceImageClass::WARNING mean activations not available for all %u layers. Image %s has %u, image %s has %u\n",scheme.getNumLayers(),imgc1.getName().c_str(),(unsigned int)imgc1.meanActivations.size(),imgc2.getName().c_str(),(unsigned int)imgc2.meanActivations.size());
  }
  //For each layer
  for(int i = 0 ; i < scheme.getNumLayers(); i++){
    int currentSize = scheme.layerSize[i];
    vector<pair<int,float> >::const_iterator it1 = imgc1.meanActivations[i].begin();
    vector<pair<int,float> >::const_iterator it2 = imgc2.meanActivations[i].begin();
    for(int j=0;j<currentSize;j++){
      float val1=0.0, val2=0.0;
      if(it1!=imgc1.meanActivations[i].end()) while(it1->first<j && it1!=imgc1.meanActivations[i].end())it1++;
      if(it2!=imgc2.meanActivations[i].end()) while(it2->first<j && it2!=imgc2.meanActivations[i].end())it2++;
      if(it1!=imgc1.meanActivations[i].end())if(it1->first == j) val1 = it1->second;
      if(it2!=imgc2.meanActivations[i].end())if(it2->first == j) val2 = it2->second;
      distance+=(val2-val1)*(val2-val1);
    }
    //printf("-Distance found on layer %u:%f\n",i,distance);
  }  
  return sqrt(distance);
}

double Util::cosineDistanceImageClass(const ImageClass &imgc1, const ImageClass &imgc2, const CNNScheme &scheme){
  double distance=0.0;
  double numerator=0.0;
  double denominator1=0.0;
  double denominator2=0.0;
  //printf("Computing distance between %s and %s\n",imgc1.getName().c_str(),imgc2.getName().c_str());
  if(scheme.getNumLayers()!= imgc1.meanActivations.size() || scheme.getNumLayers()!= imgc2.meanActivations.size()){
    printf("Util::euclideanDistanceImageClass::WARNING mean activations not available for all %u layers %u %u\n",scheme.getNumLayers(),(unsigned int)imgc1.meanActivations.size(),(unsigned int)imgc2.meanActivations.size());
  }
  //For each layer
  for(int i = 0 ; i < scheme.getNumLayers(); i++){
    int currentSize = scheme.layerSize[i];
    vector<pair<int,float> >::const_iterator it1 = imgc1.meanActivations[i].begin();
    vector<pair<int,float> >::const_iterator it2 = imgc2.meanActivations[i].begin();
    for(int j=0;j<currentSize;j++){
      float val1=0.0, val2=0.0;
      if(it1!=imgc1.meanActivations[i].end()) while(it1->first<j && it1!=imgc1.meanActivations[i].end())it1++;
      if(it2!=imgc2.meanActivations[i].end()) while(it2->first<j && it2!=imgc2.meanActivations[i].end())it2++;
      if(it1!=imgc1.meanActivations[i].end())if(it1->first == j) val1 = it1->second;
      if(it2!=imgc2.meanActivations[i].end())if(it2->first == j) val2 = it2->second;
      numerator+=val1*val2;
      denominator1+=val1*val1;
      denominator2+=val2*val2;
    }
    //printf("-Distance found on layer %u:%f\n",i,distance);
  }  
  return 1.0 - (numerator/(sqrt(denominator1)*sqrt(denominator2)));
}

//Fills the imageClasses structure with the mean activations of the images belonging to each class found in images
//According to the topology defined by the CNNScheme
//meanType == 1 arithmetic mean
//meanType == 2 harmonic mean
void Util::computeImageClasses(vector<Image> &images, const CNNScheme &scheme, vector<ImageClass> &imageClasses, int meanType){
  vector<pair<string,vector<pair<string, Image *> > > > imagesByClass;
  //Organize images by their class. For each image...
  for(vector<Image>::iterator it = images.begin(); it!=images.end(); it++){
    if((*it).getClassName().empty()){
      printf("Util::computeImageClasses::WARNING::Image %s has no imageclass name set\n",(*it).getImageName().c_str());
      continue;
    }
    string currentClass = (*it).getClassName();
    //Add image to the temporal structure associating the images of this class
    bool classFound = false;
    for(vector<pair<string,vector<pair<string, Image *> > > >::iterator it2 = imagesByClass.begin(); it2!=imagesByClass.end(); it2++){
      if(it2->first.compare(currentClass)==0){
        classFound = true;
        it2->second.push_back(pair<string, Image *> ((*it).getImageName(),&(*it)));
      }
    }
    if(!classFound){
      vector<pair<string,Image *> > newClass;
      newClass.push_back(pair<string, Image *> ((*it).getImageName(),&(*it)));
      imagesByClass.push_back(pair<string,vector<pair<string,Image *> > >(currentClass,newClass));
    }
  }
  printf("Util::computeImageClasses::Found %u image classes\n",(unsigned int)imagesByClass.size());
  //Compute each image class and store it
  #pragma omp parallel for schedule(dynamic,1)
  for(vector<pair<string,vector<pair<string,Image *> > > >::iterator it = imagesByClass.begin(); it<imagesByClass.end(); it++){
    ImageClass currentImageClass;
    currentImageClass.setName(it->first);
    for(vector<pair<string,Image *> >::iterator it2 = it->second.begin(); it2!=it->second.end();it2++){
      currentImageClass.addImageName(it2->first);
    }
    #pragma omp critical (printf)
    printf("Util::computeImageClasses::Going to compute meanAct of image class %s based on %u images\n",it->first.c_str(),(unsigned int)it->second.size());
    currentImageClass.computeMeanActivations(it->second,scheme, meanType);
    #pragma omp critical (imageClasses)
    imageClasses.push_back(currentImageClass);
  }
}

void Util::substractImageClass(ImageClass &imgc1, ImageClass &imgc2, ImageClass &result, const CNNScheme &scheme){
  result.setName(imgc1.getName()+" minus " + imgc2.getName());
  result.meanActivations.clear();
  result.norm = 0;
  printf("Substracting %s to %s\n",imgc2.getName().c_str(),imgc1.getName().c_str());
  //For each layer
  for(int i = 0; i<scheme.getNumLayers();i++){
    double layerNorm = 0;
    result.meanActivations.push_back(vector<pair<int,float> > ());
    //Struct to traverse all images concurrently, initialize at begin of each image values
    pair<int,float> * imagePointer1 = &((imgc1.meanActivations[i]).front());
    pair<int,float> * imagePointer2 = &((imgc2.meanActivations[i]).front());
    pair<int,float> * imagePointersEnd1 = &((imgc1.meanActivations[i]).back());
    pair<int,float> * imagePointersEnd2 = &((imgc2.meanActivations[i]).back());
    //Current smallest index & list of image indices with the smallest index (initialized at '0')
    int currentSmallestFeature;
    bool secondSmallest;
    //While not all pointers are done, iterate
    while(true){
      //Get new element
      currentSmallestFeature = imagePointer1->first;
      while(imagePointer2->first < imagePointer1->first){
        if(imagePointer2==imagePointersEnd2) break;
        imagePointer2++;
      }
      float combination = imagePointer1->second;
      if(imagePointer2->first == imagePointer1->first) combination-=imagePointer2->second;
      if(combination > 0) {
        result.meanActivations[i].push_back(pair<int,float>(currentSmallestFeature,combination));
        result.norm+=combination*combination;
        layerNorm+=combination*combination;
      }
      if(imagePointer1==imagePointersEnd1) break;
      imagePointer1++;
    }
    result.normByLayer.push_back(sqrt(layerNorm));
  }
  result.norm = sqrt(result.norm);
}
