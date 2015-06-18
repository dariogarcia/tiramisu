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

float Util::euclideanDistanceActivations(const Image &img1, const Image &img2){
  float distance=0; 
  //For each layer
  for(map<string,map<int,float> >::const_iterator it = img1.getActivations().begin();
    it!=img1.getActivations().end(); it++){
    //For each feature
    for(map<int,float>::const_iterator it2 = it->second.begin(); it2!=it->second.end(); it2++){
      //If layer exists in img2
      if(img2.getActivations().find(it->first)!=img2.getActivations().end()){
        //If feature exists in img2
        if(img2.getActivations()[it->first].find(it2->first)!=img2.getActivations()[it->first].end()){
          distance+= (it2->second-img2.getActivations()[it->first][it2->first])*(it2->second-img2.getActivations()[it->first][it2->first]);
        }
        //Else assume its value is 0
        else distance+= (it2->second)*(it2->second);
      }
      else distance+= (it2->second)*(it2->second);
    }
  }
  return sqrt(distance);
}
