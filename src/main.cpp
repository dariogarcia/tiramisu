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

  
//  //Print feature values
//  for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
//    it->second.exploreCNNLayer();
//  }

//  //Print layer data
//  for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
//    map<int,CNNFeature> feats = it->second.getFeatures();
//    for(map<int,CNNFeature>::iterator it2=feats.begin(); it2!=feats.end();it2++){
//      CNNFeature f = it2->second;
//      printf("%f %f %s\n",f.getMean(),f.getStdDev(),it->first.c_str());
//    }
//  }

  //Print images relevant features and why
  map<string,Image> images  = data.first;
  for(map<string,Image>::iterator it = images.begin(); it!=images.end(); it++){
    Image im = it->second;
    printf("Image %s\n",it->first.c_str());
    im.computeRelevantFeatures(data.second);
    map<string,map<int,float> > rels = im.getRelevantFeatures();
    for(map<string,map<int,float> >::iterator it2 = rels.begin(); it2!=rels.end(); it2++){
      printf(" Layer %s\n",it2->first.c_str());
      map<int,float> vals = it2->second;
      int counter = 0;
      for(map<int,float>::iterator it3 = vals.begin(); it3!=vals.end(); it3++){
        pair<float,float> stats = data.second[it2->first.c_str()].getFeatMeanAbsDev(it3->first);
        //printf("  RelFeat %u %f (real mean %f stdDev %f)\n",it3->first,it3->second,
        //stats.first,stats.second);
        counter++;
      }
      printf(" Num rel feats %u\n",counter);
    }
  }

}  

