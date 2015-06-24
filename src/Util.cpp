#include <cstdlib>
#include <ctime>
#include <math.h>

#include "../include/Util.hpp"
#include "../include/Image.hpp"

void Util::generate_random_string(char *s, const int len) {
  srand(time(0));
  static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < len; ++i) {
    s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
  }
  s[len] = 0;
}

float Util::euclideanDistanceImageActivations(const map<string,CNNLayer> &layers, const Image &img1, const Image &img2){
  float distance=0; 
  //For each layer
  for(map<string,CNNLayer>::const_iterator l_it=layers.begin(); l_it!=layers.end(); l_it++){
    //If layer does not exist in one image, error by now. TODO: Avoid error, assume al features are 0
    if(img1.getActivationsConst().find(l_it->first)==img1.getActivationsConst().end()||
        img2.getActivationsConst().find(l_it->first)==img2.getActivationsConst().end()){
      printf("Util::euclideanDistanceActivations::ERROR::Image/s do not contain layer\n");
      return 0;
    }
    const map<int,float> &act1 = img1.getActivationsConst().find(l_it->first)->second;
    const map<int,float> &act2 = img2.getActivationsConst().find(l_it->first)->second;
    for(map<int,CNNFeature>::const_iterator f_it= l_it->second.getFeaturesConst().begin(); f_it!= l_it->second.getFeaturesConst().end(); f_it++){
      float val1=0, val2=0;
      if(act1.find(f_it->first)!=act1.end()) val1 = act1.find(f_it->first)->second;
      if(act2.find(f_it->first)!=act2.end()) val2 = act2.find(f_it->first)->second;
      distance+=(val1-val2)*(val1-val2);
    } 
  }
  return sqrt(distance);
}

float Util::euclideanDistanceImageClass(const map<string,CNNLayer> &layers, const ImageClass &imgc1, const ImageClass &imgc2){
  float distance=0;
  //For each layer
  for(map<string,CNNLayer>::const_iterator l_it=layers.begin(); l_it!=layers.end(); l_it++){
    //If layer does not exist in one imageClass, error by now. TODO: Avoid error, assume al features are 0
    if(imgc1.getMeanActivations().find(l_it->first)==imgc1.getMeanActivations().end()||
        imgc2.getMeanActivations().find(l_it->first)==imgc2.getMeanActivations().end()){
      printf("Util::euclideanDistanceImageClass::ERROR::ImageClass/es do not contain layer\n");
      return 0;
    }
    const map<int,float> &mAct1 = imgc1.getMeanActivations().find(l_it->first)->second;
    const map<int,float> &mAct2 = imgc2.getMeanActivations().find(l_it->first)->second;
    //For each feature
    for(map<int,CNNFeature>::const_iterator f_it= l_it->second.getFeaturesConst().begin(); f_it!= l_it->second.getFeaturesConst().end(); f_it++){
      float val1=0, val2=0;
      if(mAct1.find(f_it->first)!=mAct1.end()) val1 = mAct1.find(f_it->first)->second;
      if(mAct2.find(f_it->first)!=mAct2.end()) val2 = mAct2.find(f_it->first)->second;
      distance+=(val1-val2)*(val1-val2);
    }
  }
  return sqrt(distance);
}


//Fills the imageClasses structure with the mean activations of the images beloning to each class found in images
//According to the topology defined by CNN
void Util::computeImageClasses(const map<string,Image> &images, const map<string,CNNLayer> &CNN,  map<string,ImageClass> &imageClasses){
  if(CNN.size()==0){
      printf("Util::computeImageClasses::WARNING::CNN has no layers. Returning no image classes\n");
  }
  map<string,vector<string> > imagesByClass;
  //Organize images by their class. For each image...
  for(map<string,Image>::const_iterator it = images.begin(); it!=images.end(); it++){
    const Image& currentImage = it->second;
    if(currentImage.getClassName().empty()){
      printf("Util::computeImageClasses::WARNING::Image %s has no imageclass name set\n",it->first.c_str());
      continue;
    }
    //Add image to the temporal structure associating the images of this class
    imagesByClass[currentImage.getClassName()].push_back(it->first);
  }
  //Compute each image class and store it
  for(map<string,vector<string> >::iterator it = imagesByClass.begin(); it!=imagesByClass.end(); it++){
    ImageClass currentImageClass;
    currentImageClass.computeMeanActivations(imagesByClass[it->first],images,CNN);
    currentImageClass.setName(it->first);
    currentImageClass.setImageNames(it->second);
    imageClasses.insert(pair<string,ImageClass> (it->first,currentImageClass));
  }
}
