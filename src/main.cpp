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
  //IO::loadImagesAndLayersFromTXTFile(argv[1],data.first, data.second);
  IO::loadLayersFromTXTFile(argv[1], data.second);
  //IO::loadImagesFromTXTFile(argv[3], data.first);
  printf("MAIN::Done loading images\n");
  //for(map<string,Image>::iterator it = data.first.begin(); it!=data.first.end(); it++){
  //  it->second.computeRelevantFeatures(data.second);
  //}
  //printf("MAIN::Done computing images relevant features\n");

  IO::writeLayersToBinaryFile("/tmp/test",data.second);
  //IO::writeImagesVerticesToTXTFile(data.first);
  //IO::writeLayersVerticesToTXTFile(data.second);
  //IO::writeImagesAndLayersEdgesToTXTFile(data.first,data.second);
  
  
//  //Print feature values
//  for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
//    it->second.exploreCNNLayer();
//  }




  IO::loadLayersFromBinaryFile("/tmp/test",data.second);

//  //Print layer data
//  for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
//    map<int,CNNFeature> feats = it->second.getFeatures();
//    for(map<int,CNNFeature>::iterator it2=feats.begin(); it2!=feats.end();it2++){
//      CNNFeature f = it2->second;
//      printf("Mean: %f StdDev:%f LayerName:%s FeatureId:%u\n",
//        f.getMean(),f.getStdDev(),it->first.c_str(),f.getId());
//    }
//  }

//  //Print images relevant features and why
//  for(map<string,Image>::iterator it = data.first.begin(); it!=data.first.end(); it++){
//    printf("Image %s\n",it->first.c_str());
//    it->second.computeRelevantFeatures(data.second);
//    map<string,map<int,float> > rels = it->second.getRelevantFeatures();
//    int total_counter = 0;
//    for(map<string,map<int,float> >::iterator it2 = rels.begin(); it2!=rels.end(); it2++){
//      printf(" Layer %s\n",it2->first.c_str());
//      map<int,float> vals = it2->second;
//      int counter = 0;
//      for(map<int,float>::iterator it3 = vals.begin(); it3!=vals.end(); it3++){
//        pair<float,float> stats = data.second[it2->first.c_str()].getFeatMeanAbsDev(it3->first);
//        //printf("  RelFeat %u %f (real mean %f stdDev %f)\n",it3->first,it3->second,
//        //stats.first,stats.second);
//        counter++;
//      }
//      total_counter += counter;
//      printf(" Num rel feats %u\n",counter);
//    }
//    printf(" Total rel feats %u\n",total_counter);
//  }

}  

