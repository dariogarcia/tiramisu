#include <cstdlib>
#include <ctime>

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

//float Util:euclideanDistance(Image img1, Image img2){
//  float distance=0; 
//  
//
//  return distance;
//}
