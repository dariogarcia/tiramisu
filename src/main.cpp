#include <dirent.h>
#include <cstring>
#include <map>

#include "../include/Image.hpp"
#include "../include/ImageClass.hpp"
#include "../include/CNNLayer.hpp"
#include "../include/IO.hpp"
#include "../include/Util.hpp"

using std::map;
using std::pair;

int main(int argc, char* argv[]){
  pair<map<string,Image>,map<string,CNNLayer> > data;
  IO::loadImagesAndLayersFromTXTFile(argv[1],data.first, data.second);
  //IO::loadLayersFromTXTFile(argv[1], data.second);
  //IO::loadImagesFromTXTFile(argv[3], data.first);
  printf("MAIN::Done loading images\n");

  ImageClass ic;

  time_t t_init;
  time(&t_init);
  ic.computeMeanActivations(data.first, data.second);
  time_t t_end;
  time(&t_end);
  double t_diff = difftime (t_end,t_init);
  printf("MAIN::Done computing mean activations of image class in %f\n",t_diff);
  

  //COMPUTE IMAGES RELEVANT FEATURES
  //for(map<string,Image>::iterator it = data.first.begin(); it!=data.first.end(); it++){
  //  it->second.computeRelevantFeatures(data.second);
  //}
  //printf("MAIN::Done computing images relevant features\n");

  //GENERATE A RANDOM NAME
  //char rand_name[5];
  //Util::generate_random_string(rand_name,5);
  //string filename = string(argv[1])+"somefile_"+string(rand_name)+".tir";

  //IO OPERATIONS
  //IO::writeLayersToBinaryFile("/tmp/tira_test",data.second);
  //printf("Done writting, lets read\n");
  //map<string,CNNLayer> read_layers;
  //IO::loadLayersFromBinaryFile("/tmp/tira_test",read_layers);
  //IO::writeImagesVerticesToTXTFile(data.first);
  //IO::writeLayersVerticesToTXTFile(data.second);
  //IO::writeImagesAndLayersEdgesToTXTFile(data.first,data.second);
  
  
//  //PRINT FEATURE VALUES
//  for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
//    it->second.exploreCNNLayer();
//  }


  //PRINT LAYER DATA
  //for(map<string,CNNLayer>::iterator it = read_layers.begin();it!=read_layers.end();it++){
  //  map<int,CNNFeature> feats = it->second.getFeatures();
  //  //printf("layer %s has %u features\n",it->first.c_str(),feats.size());
  //  for(map<int,CNNFeature>::iterator it2=feats.begin(); it2!=feats.end();it2++){
  //    CNNFeature f = it2->second;
  //    printf("Mean: %f StdDev:%f LayerName:%s FeatureId:%u\n",
  //      f.getMean(),f.getStdDev(),it->first.c_str(),f.getId());
  //  }
  //}

//  //PRINT IMAGES RELEVANT FEATURES AND WHY
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

