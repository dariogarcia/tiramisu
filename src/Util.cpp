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

float Util::euclideanDistanceImageClass(const ImageClass &imgc1, const ImageClass &imgc2, const CNNScheme &scheme){
  float distance=0.0;
  //For each layer
  for(int i = 0 ; i < scheme.getNumLayers(); i++){
    int currentSize = scheme.layerSize[i];
    vector<pair<int,float> >::const_iterator it1 = imgc1.meanActivations[i].begin();
    vector<pair<int,float> >::const_iterator it2 = imgc2.meanActivations[i].begin();
    for(int j=0;j<currentSize;j++){
      float val1=0.0, val2=0.0;
      while(it1->first<j)it1++;
      while(it2->first<j)it2++;
      if(it1->first == j) val1 = it1->second;
      if(it2->first == j) val2 = it2->second;
      distance+=fabs(val1-val2)*fabs(val1-val2);
    }
  }  
  return sqrt(distance);
}

//Fills the imageClasses structure with the mean activations of the images belonging to each class found in images
//According to the topology defined by the CNNScheme
void Util::computeImageClasses(vector<Image> &images, const CNNScheme &scheme, vector<ImageClass> &imageClasses){
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
  printf("Util::computeImageClasses::Found %u image classes\n",imagesByClass.size());
  //Compute each image class and store it
  #pragma omp parallel for schedule(dynamic,1)
  for(vector<pair<string,vector<pair<string,Image *> > > >::iterator it = imagesByClass.begin(); it<imagesByClass.end(); it++){
    ImageClass currentImageClass;
    currentImageClass.setName(it->first);
    for(vector<pair<string,Image *> >::iterator it2 = it->second.begin(); it2!=it->second.end();it2++){
      currentImageClass.addImageName(it2->first);
    }
    currentImageClass.computeMeanActivations(it->second,scheme);
    #pragma omp critical (imageClasses)
    imageClasses.push_back(currentImageClass);
    #pragma omp critical (printf)
    printf("Util::computeImageClasses::Done computing meanAct of image class %s based on %u images\n",it->first.c_str(),it->second.size());
  }
}
