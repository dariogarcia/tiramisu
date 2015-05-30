#include <dirent.h>
#include <cstring>
#include <map>

#include "../include/Image.hpp"
#include "../include/CNNLayer.hpp"
#include "../include/IO.hpp"

using std::map;
using std::pair;

int main(int argc, char* argv[]){
  pair<map<string,Image>,map<string,CNNLayer> > data;
  data = IO::loadDirectoryOfImages(argv[1]);

  for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
    map<int,CNNFeature> feats = it->second.getFeatures();
    for(map<int,CNNFeature>::iterator it2=feats.begin(); it2!=feats.end();it2++){
      CNNFeature f = it2->second;
      printf("%f %f %s\n",f.getMean(),f.getStdDev(),it->first.c_str());
    }
  }
}  

