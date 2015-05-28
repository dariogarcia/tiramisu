#include "../include/imagelayersdata.hpp"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <fstream>

#include <limits.h>       //For PATH_MAX

using namespace tiramisu;
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::sort;
using std::cout;
using std::endl;
using std::min;

typedef map<string,map<int,float> > ImageLayer;

struct Feature {
  vector<float> values;
  int counter;
  float mean;
  float absoluteDev;
  float standardDev;
  float relevance;
  Feature(){
    counter=0;
  };
  Feature(float val){
    values.push_back(val);
    counter = 1;
  };
  void update(float val){
    values.push_back(val);
    counter++;
  };
  void computeMean(){
    if(counter!=values.size()) cout<<"ERROR: Feature counter is different than num values"<<endl;
    mean = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) mean=mean+*i;
    mean = mean/counter;
  };
  void computeAbsDev(){
    absoluteDev = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      absoluteDev=absoluteDev+abs(*i-mean);   
    }
    absoluteDev = absoluteDev/counter;
  };
  void computeStdDev(){
    standardDev = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      standardDev=standardDev+((*i-mean)*(*i-mean));   
    }
    standardDev = sqrt(standardDev/counter);
  };
  void computeRelevance(){
    relevance = mean - standardDev;
  };
}; // Feature
  
struct sort_by_relevance{
  bool operator()(const Feature& x, const Feature& y){
    return x.relevance>y.relevance;
  }
};


typedef map<int,Feature> Features;

//struct Similarity { 
//    string name1; 
//    string name2; 
//    float similarity; 
//}; //  Similarity 
//
//struct sort_by_difference{
//  bool operator()(const Similarity& x, const Similarity& y){
//    return x.similarity>y.similarity;
//  }
//};
//
//struct TotalFeature {
//    string name;
//    float total;
//};
//
//struct sort_by_total{
//  bool operator()(const TotalFeature & x, const TotalFeature & y){
//    return x.total>y.total;
//  }
//};
  
int main(int argc, char* argv[]){
  struct dirent *pDirent;
  DIR *pDir;
  map<string,ImageLayersData> images;
  vector<pair<map<string,Features>,string> > global_feats; 
  for(int i = 1; i<argc; i++){
    pDir = opendir(argv[i]);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", argv[i]);
        return 1;
    }
    while ((pDirent = readdir(pDir)) != NULL) {
      if(std::strcmp(pDirent->d_name,".")==0)continue;
      if(std::strcmp(pDirent->d_name,"..")==0)continue;
      struct dirent *pDirent2;
      DIR *pDir2;
      pDir2 = opendir((argv[i]+string("/")+string(pDirent->d_name)).c_str());
      if (pDir2 == NULL) {
          printf ("Cannot open sub-directory '%s'\n", (argv[i]+'/'+string(pDirent->d_name)).c_str());
          return 1;
      }
      while ((pDirent2 = readdir(pDir2)) != NULL) {
        if(std::strcmp(pDirent2->d_name,".")==0)continue;
        if(std::strcmp(pDirent2->d_name,"..")==0)continue;
        string fullpath = (argv[i]+string("/")+string(pDirent->d_name)+string("/")+string(pDirent2->d_name)).c_str();
        std::cout<<"Reading feature file "<<fullpath<<std::endl;
        string type = string(pDirent2->d_name).substr(string(pDirent2->d_name).find_last_of("_"));
        string name = string(pDirent2->d_name).substr(0,string(pDirent2->d_name).find_last_of("_"));
        if(images.find(name)==images.end()) {
          images.insert(std::make_pair(name,ImageLayersData()));
        }
        ImageLayersData current = images.find(name)->second;
        current.loadLayersFromFile(fullpath,type);
        images[name] = current;
      }
      closedir(pDir2);
    }
    closedir (pDir);
    cout<<"Total loaded images:"<<images.size()<<endl;

    //CODE TO CHECK NUMBER AND SIZE OF LOADED FILES
    //for(map<string,ImageLayersData>::iterator it = images.begin(); it!= images.end(); it++){
    //  std::cout<<"-Read "<<it->first<<endl;
    //  ImageLayersData cur = it->second;
    //  for(map<string,map<int,float> >::iterator it2=cur.getLayersData().begin(); it2!=cur.getLayersData().end(); it2++){
    //    std::cout<<"--Val "<<it2->first<<endl;
    //    std::cout<<"--Siz "<<it2->second.size()<<endl;
    //  }
    //} 
    
    //LOAD FEATURES FROM IMAGES
    map<string,Features> all_feats;
    //For each image processed
    for(map<string,ImageLayersData>::iterator image = images.begin(); image!= images.end(); image++){
      ImageLayersData imageData = image->second;
      //For each layer of the image
      for(ImageLayer::iterator layerIt=imageData.getLayersData().begin(); layerIt!=imageData.getLayersData().end(); layerIt++){
        map<int,float> imageLayerFeatures = layerIt->second;
        //If the layer is new, add it
        if(all_feats.find(layerIt->first)==all_feats.end()) all_feats.insert(std::make_pair(layerIt->first,Features()));
        //For each feature in the layer
        for(map<int,float>::iterator feat=imageLayerFeatures.begin();feat!=imageLayerFeatures.end();feat++){
          //If the feature is new, add it
          if(all_feats[layerIt->first].find(feat->first)==all_feats[layerIt->first].end()){
            all_feats[layerIt->first].insert(std::make_pair(feat->first,Feature(feat->second)));
          }
          //Otherwise update its values
          else{
            all_feats[layerIt->first][feat->first].update(feat->second);       
            //all_feats[layerIt->first][feat->first]=all_feats[layerIt->first][feat->first].update(feat->second);       
          }
        }
      }
    }

    //VALIDATE NUM FEATURES LOADED
    for(map<string,Features>::iterator it=all_feats.begin(); it!=all_feats.end();it++){
      cout<<"reading feature :"<<it->first<<" having num elems:"<<it->second.size()<<endl;
      cout<<it->second[1].counter<<" "<<it->second[1].values.size()<<" "<<it->second[1].values[10]<<endl;   
    }     

    //Compute means and deviations
    std::ofstream myfile;
    myfile.open ("mean_features.txt"); 
    for(map<string,Features>::iterator it=all_feats.begin(); it!=all_feats.end();it++){
      for(Features::iterator it2=it->second.begin();it2!=it->second.end();it2++){
        it2->second.computeMean();
        it2->second.computeStdDev();
        it2->second.computeAbsDev();
        it2->second.computeRelevance();
        myfile<<it2->second.relevance<<" ";
        myfile<<it2->second.mean<<" ";
        myfile<<it2->second.standardDev<<" ";
        myfile<<it2->second.absoluteDev<<" ";
        myfile<<it->first<<" ";
        myfile<<it2->first<<endl;
      }
    }
    myfile.close();
    global_feats.push_back(std::make_pair(all_feats,argv[i]));
  } 

  //For every pair of sets
  std::ofstream myfile2;
  myfile2.open ("similarities.txt"); 
  for(int i=0;i<global_feats.size();i++){
    for(int j=i+1; j<global_feats.size();j++){
      //Check the features that are most similar and diverge the most from the rest
      myfile2<<global_feats[i].second<<" "<<global_feats[j].second<<endl;
      for(map<string,Features>::iterator it=(global_feats[i]).first.begin(); it!=(global_feats[i]).first.end();it++){
        for(Features::iterator it2=it->second.begin();it2!=it->second.end();it2++){
          float first_rel = it2->second.relevance;
          if(first_rel<=10)continue;
          float second_rel = ((global_feats[j].first)[it->first])[it2->first].relevance;
          if(second_rel<=10)continue;
          for(int k=0; k<global_feats.size();k++){
            if(k==i || k==j) continue;
            float other_rel = ((global_feats[k].first)[it->first])[it2->first].relevance;
            if((other_rel*2 < first_rel) && (other_rel*2 < second_rel)) {
              myfile2<<first_rel<<" "<<second_rel<<" "<<other_rel<<" "<<it->first<<" "<<it2->first<<endl;
            }
          }
        } 
      }  
    }
  }
  myfile2.close();

  //map<string,map<int,float> > ImageLayer;

  //vector<Similarity> sims;
  //for(int i = 0 ; i<images.size(); i++){
  //  ImageLayersData tmp = images[i];
  //  for(int j = i+1 ; j<images.size() ; j++){
  //    ImageLayersData tmp2 = images[j];
  //    Similarity sim;
  //    sim.name1=tmp.getName();
  //    sim.name2=tmp2.getName();
  //    float partial_sim = 0;
  //    for(std::map<int,float>::iterator it = tmp.getLayersData().begin(); it != tmp.getLayersData().end(); it++){
  //      //if((it->second)<40 || (tmp2.getLayersData()[it->first])<40) continue;
  //      
  //      //float diff = (it->second)-(tmp2.getLayersData()[it->first]);
  //      //if(diff<0)diff = diff*-1;
  //      //float diff = min(it->second,tmp2.getLayersData()[it->first])-50-((it->second)-(tmp2.getLayersData()[it->first]));
  //      float diff = min(it->second,tmp2.getLayersData()[it->first]);
  //      if(diff<0)continue;
  //      partial_sim+=diff;
  //    }
  //    sim.similarity=partial_sim;
  //    sims.push_back(sim);
  //    //std::cout<<sim.name1<<" "<<sim.name2<<" "<<sim.similarity<<std::endl;
  //  }
  //}
  //sort_by_difference sort_op;
  //sort(sims.begin(), sims.end(),sort_op);

  //vector<TotalFeature> totals;
  //for(int i = 0; i<images.size(); i++){
  //  ImageLayersData p = images[i];
  //  float accumulated = 0;
  //  for(int j=0; j<p.getLayersData().size();j++){
  //      accumulated+=p.getLayersData()[j];
  //  }
  //  TotalFeature tf;
  //  tf.name = p.getName();
  //  tf.total = accumulated;
  //  totals.push_back(tf);
  //}

  //sort_by_total sort_total;
  //sort(totals.begin(),totals.end(),sort_total);

  //for(int i = 0 ; i<totals.size(); i++){
  //  //cout<<totals[i].name<<" "<<totals[i].total<<endl;
  //}

  //for(int i = 0 ; i<40; i++){
  //  cout<<sims[i].name1<<" "<<sims[i].name2<<" "<<sims[i].similarity<<endl;
  //}

}
