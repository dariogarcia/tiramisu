#include <dirent.h>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>

#include "../include/IO.hpp"
#include "../include/Util.hpp"
#include "../include/CNNScheme.hpp"
#include "../include/ImageClass.hpp"

using std::set;
using std::vector;
using std::pair;

int main(int argc, char* argv[]){
  CNNScheme scheme;
  CNNFeatures cnnfeatures;
  vector<Image> images;
  
  time_t t_init;
  time(&t_init);
  IO::loadSchemeFromTXTFile(argv[1], scheme);
  time_t t_end;
  time(&t_end);
  double t_diff = difftime (t_end,t_init);
  printf("MAIN::Load scheme took %f\n",t_diff);

  scheme.printScheme();

  //cnnfeatures.initialize(scheme);
  //printf("Done initializing\n");
  //time(&t_init);
  //IO::loadFeaturesFromTXTFile(argv[1], cnnfeatures, scheme);
  //time(&t_end);
  //printf("Load features took %f\n",difftime (t_end,t_init));

  
  time(&t_init);
  IO::loadImagesFromTXTFile(argv[1], images, scheme);
  time(&t_end);
  printf("MAIN::Load images took %f\n",difftime (t_end,t_init));

  //time(&t_init);
  //for(int i = 0 ; i<images.size();i++) images[i].normalizeActivations();
  //time(&t_end);
  //printf("Normalizing images took %f\n",difftime (t_end,t_init));
 
  time(&t_init);
  IO::readAndSetImageClasses(argv[2], images);
  time(&t_end);
  printf("MAIN::Read & set image classes took %f\n",difftime (t_end,t_init));
  
  time(&t_init);
  vector<ImageClass> imageClasses;
  int meanType = 1;
  //int meanType = 2;
  if(meanType==1)printf("MAIN::Using arithmetic mean\n");
  if(meanType==2)printf("MAIN::Using harmonic mean\n");
  Util::computeImageClasses(images, scheme, imageClasses, meanType);
  time(&t_end);
  printf("MAIN::Compute image classes took %f\n",difftime (t_end,t_init));


  time(&t_init);
  int normType = 2;
  if(normType==1)printf("MAIN::Normalizing overall vector\n");
  if(normType==2)printf("MAIN::Normalizing vector by CNN layer\n");
  for(int i = 0 ; i<imageClasses.size();i++) imageClasses[i].normalizeMeanActivations(normType);
  time(&t_end);
  printf("MAIN::Normalizing image classes took %f\n",difftime (t_end,t_init));


  time(&t_init);
  //int distanceType = 1;
  int distanceType = 2;
  if(distanceType==1)printf("MAIN::Using euclidean distance\n");
  if(distanceType==2)printf("MAIN::Using cosine distance\n");
  for(vector<ImageClass>::iterator it = imageClasses.begin(); it!=imageClasses.end(); it++){
    //distanceType = 1 -> euclidean distance
    //distanceType = 2 -> cosine distance
    pair<ImageClass,double> closest = (*it).findClosestClass(imageClasses, scheme, distanceType);
    printf("CLOSEST::Closest class to %s is %s at distance %f\n",(*it).getName().c_str(),closest.first.getName().c_str(),closest.second);
  }
  time(&t_end);
  printf("MAIN::Compute image class distances took %f\n",difftime (t_end,t_init));


  time(&t_init);
  typedef pair<ImageClass,set<int> > ArithmeticIC;
  ImageClass teddy;
  ImageClass bear;
  ArithmeticIC teddy_minus_bear;
  for(int i = 0 ; i<imageClasses.size();i++) {
    if(imageClasses[i].getName().compare("n04399382 teddy, teddy bear")==0) {
      teddy = imageClasses[i];
      teddy_minus_bear.second.insert(i);
    }
    if(imageClasses[i].getName().compare("n02132136 brown bear, bruin, Ursus arctos")==0) {
      bear = imageClasses[i];
      teddy_minus_bear.second.insert(i);
    }
  }
  Util::substractImageClass(teddy,bear,teddy_minus_bear.first,scheme);
  time(&t_end);
  printf("MAIN::Compute arithmetics took %f\n",difftime (t_end,t_init));


  //The whole ArithmeticImageClass code is not elegant or efficient. Needs work (get rid of sets, for starters)
  if(distanceType==1)printf("MAIN::Using euclidean distance\n");
  if(distanceType==2)printf("MAIN::Using cosine distance\n");
  vector<ImageClass> tempIC = imageClasses;
  for(int i = teddy_minus_bear.second.size(); i>0 ;i--) tempIC.erase(tempIC.begin()+(*std::next(teddy_minus_bear.second.begin(), i-1)));
  pair<ImageClass,double> closestIC = teddy_minus_bear.first.findClosestClass(tempIC, scheme, distanceType);
  printf("CLOSEST::Closest class to %s is %s at distance %f\n",teddy_minus_bear.first.getName().c_str(),closestIC.first.getName().c_str(),closestIC.second);
  time(&t_end);
  printf("MAIN::Compute closest classes to arithmetic classes took %f\n",difftime (t_end,t_init));


///////////////////////////////////////////////////////////
////EXAMPLES FOR REFERENCE. DO NOT MODIFY. COPY & REUSE////
///////////////////////////////////////////////////////////

  //LOAD TXT (CAFFE OUTPUT) DATA
  //Load CNN layers, feature, and their activations, from txt files (directory/directory/layers_activations)
  //Computes statistics for each feature (mean, absolute deviation, standard deviation, activation threshold)
  //IO::loadLayersFromTXTFile(argv[1], data.second);

  //Load images activations from txt files  (directory/directory/layers_activations). 
  //IO::loadImagesFromTXTFile(argv[1], data.first);
  
  //Load images & CNN layers (features, stats, etc.) from the same set of txt files. May be a biased approach.
  //IO::loadImagesAndLayersFromTXTFile(argv[1],data.first, data.second);


  //LOAD CLASS LABEL DATA (PREVIOUSLY CURATED)  
  //Read image classes names from a two column txt file for previously loaded images. Updates image data
  //IO::readAndSetImageClasses(argv[1], data.first);


  //STORE GRAPH-LIKE DATA
  //Write images as vertices in a txt file (one column file of image names)
  //IO::writeImagesVerticesToTXTFile(argv[1],data.first);

  //Write CNN features as vertices in a txt file (one column file of feature ids)
  //IO::writeLayersVerticesToTXTFile(argv[1],data.second);

  //Write both images and features as vertices in txt file (one column, image names and feature ids)
  //IO::writeImagesAndLayersVerticesToTXTFile(argv[1], data.first, data.second);

  //Write both images and features as edges in txt file (two columns, image names - relevant features ids)
  //IO::writeImagesAndLayersEdgesToTXTFile(argv[1],data.first,data.second);


  //STORE BINARY DATA
  //Store basic CNN data (layers, features, and statistics) to a binary file
  //IO::writeLayersToBinaryFile(argv[1],data.second);

  //Store ImageClass data to a binary file
  //IO::writeImageClassToBinaryFile(argv[1],imageclass);


  //LOAD BINARY DATA (PREVIOUSLY GENERATED BY TIRAMISU)
  //Load CNN layers (features, stats, etc.) from a binary file previously generated.
  //IO::loadLayersFromBinaryFile(argv[1],data.second);


  //OPERATIONS ON IMAGES
  //Computes the relevant features (those beyond activation threshold) of images
  //for(map<string,Image>::iterator it = data.first.begin(); it!=data.first.end(); it++){
  //  it->second.computeRelevantFeatures(data.second);
  //}

  //Find the euclidean distance between the raw activations of two images
  //Image img1,img2;
  //float euclDist = Util::euclideanDistanceImageActivations(data.second, img1, img2);



  //OPERATIONS ON IMAGE CLASSES
  //Generate one image class representation by computing the mean activations of a set of images (labeled or unlabeled)
  //ImageClass imageclass;
  //imageclass.computeMeanActivations(data.first, data.second);

  //Find the closest image class by euclidean distance to one from within a set
  //vector<ImageClass> iClassCandidates;
  //ImageClass closest = imageclass.findClosestClassByEucliDist(iClassCandidates, data.second);
  //map<string,ImageClass> iClassCandidates;
  //ImageClass closest = imageclass.findClosestClassByEucliDist(iClassCandidates, data.second);

  //Find the euclidean distance between two image classes
  //ImageClass imgc1, imgc2;
  //float euclDist = Util::euclideanDistanceImageClass(data.second, imgc1, imgc2);

  //Build all image classes from a set of labeled images
  //map<string,ImageClass> imageClasses;
  //Util::computeImageClasses(data.first, data.second , imageClasses);




  //PRINT INTERMEDIATE DATA
  //Print features data 
  //for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
  //  it->second.exploreCNNLayer();
  //}

  //Print layer data
  //for(map<string,CNNLayer>::iterator it = data.second.begin();it!=data.second.end();it++){
  //  map<int,CNNFeature> feats = it->second.getFeaturesDyn();
  //  //printf("layer %s has %u features\n",it->first.c_str(),feats.size());
  //  for(map<int,CNNFeature>::iterator it2=feats.begin(); it2!=feats.end();it2++){
  //    CNNFeature f = it2->second;
  //    printf("Mean: %f StdDev:%f LayerName:%s FeatureId:%u\n",
  //      f.getMean(),f.getStdDev(),it->first.c_str(),f.getId());
  //  }
  //}

  //Print relevant features of images
  //for(map<string,Image>::iterator it = data.first.begin(); it!=data.first.end(); it++){
  //  printf("Image %s\n",it->first.c_str());
  //  it->second.computeRelevantFeatures(data.second);
  //  map<string,map<int,float> > rels = it->second.getRelevantFeaturesDyn();
  //  int total_counter = 0;
  //  for(map<string,map<int,float> >::iterator it2 = rels.begin(); it2!=rels.end(); it2++){
  //    printf(" Layer %s\n",it2->first.c_str());
  //    map<int,float> vals = it2->second;
  //    int counter = 0;
  //    for(map<int,float>::iterator it3 = vals.begin(); it3!=vals.end(); it3++){
  //      pair<float,float> stats = data.second[it2->first.c_str()].getFeatMeanAbsDev(it3->first);
  //      //printf("  RelFeat %u %f (real mean %f stdDev %f)\n",it3->first,it3->second,
  //      //stats.first,stats.second);
  //      counter++;
  //    }
  //    total_counter += counter;
  //    printf(" Num rel feats %u\n",counter);
  //  }
  //  printf(" Total rel feats %u\n",total_counter);
  //}


  //UTIL (not tiramisu-specific)
  //Generate random string
  //char rand_name[5];
  //Util::generate_random_string(rand_name,5);
  //string filename = string(argv[1])+"somefile_"+string(rand_name)+".tir";

  //Calculate computing time
  //time_t t_init;
  //time(&t_init);
  //time_t t_end;
  //time(&t_end);
  //double t_diff = difftime (t_end,t_init);
  //printf("%f\n",t_diff);
  
}  

