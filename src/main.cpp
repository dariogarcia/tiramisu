#include <dirent.h>
#include <cstring>
#include <map>

#include "../include/Image.hpp"
#include "../include/CNNLayer.hpp"

using std::vector;
using std::map;
using std::strcmp;
using std::pair;


//Method to explore and print contents of a set of images
//Intended for validation of loading process
void exploreImages(const map<string,Image> &images){
  int imageCounter = 0;
  for(map<string,Image>::const_iterator it = images.begin() ; it != images.end(); it++){
    Image currentImage = it->second;
    printf("-Image num: '%u', name '%s', path '%s', has '%lu' activations \n",
      imageCounter, currentImage.getName().c_str(),currentImage.getPath().c_str(), 
      currentImage.getActivations().size());
    int activationCounter = 0;
    for(map<string,map<int,Activation> >::iterator it2 = currentImage.getActivations().begin(); 
      it2!=currentImage.getActivations().end() ; it2++){
      map<int,Activation> currentActivation = it2->second;
      printf("--Activation num: '%u', name '%s', has '%lu' values \n",
        activationCounter, it2->first.c_str(), currentActivation.size());
      int valueCounter = 0;
      for(map<int,Activation>::iterator it3 = currentActivation.begin(); it3!=currentActivation.end();
        it3++){ 
        printf("---Value num: '%u', is '%f' for layer '%s' and feature '%u'\n",
          valueCounter,it3->second.getValue(),it3->second.getCNNLayerId().c_str(),it3->second.getFeatureId());
        valueCounter++;
      }
      activationCounter++;
    }
    imageCounter++;
  }
}

//Method to explore and print contents of a set of CNNLayers
//Intended for validation of loading process
void exploreCNNLayers(const map<string,CNNLayer> &layers){
  int layerCounter = 0;
  for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
    CNNLayer currentLayer = it->second;
    map<int,CNNFeature> currentFeatures = currentLayer.getFeatures();
    printf("-Layer num: '%u', name '%s', has '%lu' activations \n",
      layerCounter, currentLayer.getName().c_str(), currentFeatures.size());
    int featureCounter = 0;
    for(map<int,CNNFeature>::iterator it2 = currentFeatures.begin(); 
      it2!=currentFeatures.end(); it2++){
      CNNFeature currentFeature = it2->second;
      printf("--Feature num: '%u', with Id '%u':'%u', has mean '%f', stdDev '%f', absDev '%f' and '%lu' values\n",
        featureCounter,currentFeature.getId(),it2->first,currentFeature.getMean(),
        currentFeature.getStdDev(),currentFeature.getAbsDev(),currentFeature.getValues().size());
      featureCounter++;
      vector<float> values = currentFeature.getValues();
      int valueCounter = 0;
      for(vector<float>::iterator it3 = values.begin(); it3!=values.end(); it3++){
        printf("---Value num: '%u' is '%f'\n",valueCounter,*it3);
        valueCounter++;
      }
    }
    layerCounter++;
  }  
}


void computeLayersStatistics(map<string,CNNLayer> &layers){
  for(map<string,CNNLayer>::iterator it = layers.begin();it!=layers.end();it++){
    it->second.computeFeatureStatistics();
  }
}

int main(int argc, char* argv[]){
  struct dirent *pDirent;
  DIR *pDir;
  map<string,Image> images;
  map<string,CNNLayer> layers;
  for(int i = 1; i<argc; i++){
    pDir = opendir(argv[i]);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", argv[i]);
        return 1;
    }
    //For each directory in the input path
    while ((pDirent = readdir(pDir)) != NULL) {
      //if(strcmp(pDirent->d_name,".")==0)continue;
      //if(strcmp(pDirent->d_name,"..")==0)continue;
      if(string(pDirent->d_name).find(".")!= string::npos) continue;
      struct dirent *pDirent2;
      DIR *pDir2;
      pDir2 = opendir((argv[i]+string("/")+string(pDirent->d_name)).c_str());
      if (pDir2 == NULL) {
          printf ("Cannot open sub-directory '%s'\n", 
            (argv[i]+'/'+string(pDirent->d_name)).c_str());
          continue;
      }
      //Read each file
      while ((pDirent2 = readdir(pDir2)) != NULL) {
        if(strcmp(pDirent2->d_name,".")==0)continue;
        if(strcmp(pDirent2->d_name,"..")==0)continue;
        if(string(pDirent2->d_name).find(".")!= string::npos) continue;
        string fullpath = (argv[i]+string(pDirent->d_name)+
          string("/")+string(pDirent2->d_name)).c_str();
        printf ("Processing feature file '%s'\n", fullpath.c_str());
        string layerName = string(pDirent2->d_name)
          .substr(string(pDirent2->d_name).find_last_of("_")+1);
        string imageName = string(pDirent2->d_name)
          .substr(0,string(pDirent2->d_name).find_last_of("_"));
        //If new layer, set name and store
        CNNLayer currentLayer;
        if(layers.find(layerName)==layers.end()){
          currentLayer.setName(layerName);
          layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
        }
        currentLayer = layers[layerName];
        currentLayer.addFeatures(fullpath);
        layers[layerName] = currentLayer;
        //If new image, set name and path and store
        Image currentImage;
        if(images.find(imageName)==images.end()) {
          currentImage.setName(imageName);
          currentImage.setPath(argv[i]+string(pDirent->d_name));
          images.insert(pair<string,Image>(imageName,currentImage));
        }
        currentImage = images[imageName];
        currentImage.addActivations(fullpath,layerName); 
        images[imageName] = currentImage;
      }
      closedir(pDir2);
    }
    closedir (pDir);
    printf ("Total loaded images: '%lu'\n", images.size());
  }

  exploreCNNLayers(layers);
  computeLayersStatistics(layers);
  exploreCNNLayers(layers);
}


