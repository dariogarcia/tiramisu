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

float Util::euclideanDistanceActivations(const map<string,CNNLayer> &layers, const Image &img1, const Image &img2){
  float distance=0; 
  //For each layer
  for(map<string,CNNLayer>::const_iterator l_it=layers.begin(); l_it!=layers.end(); l_it++){
    //If layer does not exist in one image, error by now. TODO: Avoid error, assume al features are 0
    if(img1.getActivations().find(l_it->first)==img1.getActivations().end()||
        img2.getActivations().find(l_it->first)==img2.getActivations().end()){
      printf("Util::euclideanDistanceActivations::ERROR::Image/s do not contain layer\n");
      return 0;
    }
    map<int,float> &act1 = img1.getActivations().find(l_it->first)->second;
    map<int,float> &act2 = img2.getActivations().find(l_it->first)->second;
    for(map<int,CNNFeature>::const_iterator f_it= l_it->second.getFeatures().begin(); f_it!= l_it->second.getFeatures().end(); f_it++){
      float val1=0, val2=0;
      if(act1.find(f_it->first)!=act1.end()) val1 = act1[f_it->first];
      if(act2.find(f_it->first)!=act2.end()) val2 = act2[f_it->first];
      distance+=(val1-val2)*(val1-val2);
    } 
  }
  return sqrt(distance);
}
