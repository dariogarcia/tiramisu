#include <dirent.h>
#include <cstring>
#include <map>

#include "../include/IO.hpp"
#include "../include/Image.hpp"
#include "../include/CNNLayer.hpp"

using std::map;

//Loads a directory containing directories of one or more images.
//It builds the images, features and layers data structures and statistics
void IO::loadImagesAndLayers(string path, map<string,Image> &images, map<string,CNNLayer> &layers){
  struct dirent *pDirent;
  DIR *pDir;
  pDir = opendir(path.c_str());
  if (pDir == NULL) {
    printf ("IO::loadImagesAndLayers::Cannot open directory '%s'\n", path.c_str());
    return;
  }
  //For each directory in the input path
  while ((pDirent = readdir(pDir)) != NULL) {
    if(string(pDirent->d_name).find(".")!= string::npos) continue;
    struct dirent *pDirent2;
    DIR *pDir2;
    pDir2 = opendir((path+string("/")+string(pDirent->d_name)).c_str());
    //pDir2 = opendir(path.c_str());
    if (pDir2 == NULL) {
        printf ("IO::loadImagesAndLayers::Cannot open sub-directory '%s'\n", 
          (path+string("/")+string(pDirent->d_name)).c_str());
        continue;
    }
    //Read each file
    while ((pDirent2 = readdir(pDir2)) != NULL) {
      if(string(pDirent2->d_name).find(".")!= string::npos) continue;
      string fullpath = (path+string(pDirent->d_name)+
        string("/")+string(pDirent2->d_name)).c_str();
      //printf ("IO::loadDirectoryOfImages::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      //If new layer, set name and store
      CNNLayer currentLayer;
      if(layers.find(layerName)==layers.end()){
        printf ("IO::loadImagesAndLayers::Adding new layer '%s'\n", layerName.c_str());
        currentLayer.setName(layerName);
        layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
      }
      currentLayer = layers[layerName];
      currentLayer.addFeatures(fullpath);
      layers[layerName] = currentLayer;
      //If new image, set name and path and store
      Image currentImage;
      if(images.find(imageName)==images.end()) {
        printf ("IO::loadImagesAndLayers::Adding new image '%s'\n", imageName.c_str());
        currentImage.setName(imageName);
        currentImage.setPath(path+string(pDirent->d_name));
        images.insert(pair<string,Image>(imageName,currentImage));
      }
      currentImage = images[imageName];
      currentImage.addActivations(fullpath,layerName); 
      images[imageName] = currentImage;
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadImagesAndLayers::Total loaded images: '%lu'\n", images.size());
  //Once all images have been loaded, compute the layer statistics
  for(map<string,CNNLayer>::iterator it=layers.begin();it!=layers.end();it++){
    it->second.computeLayerStatistics();
    printf ("IO::loadImagesAndLayers::Done computing statistics of layer %s\n",it->second.getName().c_str());
  }
  printf ("IO::loadImagesAndLayers::Done computing all layer statistics\n");
}


//Loads a directory containing directories of one or more images.
//It builds the images data structures and statistics
void IO::loadImages(string path, map<string,Image> &images){
  struct dirent *pDirent;
  DIR *pDir;
  pDir = opendir(path.c_str());
  if (pDir == NULL) {
    printf ("IO::loadImages::Cannot open directory '%s'\n", path.c_str());
    return;
  }
  //For each directory in the input path
  while ((pDirent = readdir(pDir)) != NULL) {
    if(string(pDirent->d_name).find(".")!= string::npos) continue;
    struct dirent *pDirent2;
    DIR *pDir2;
    pDir2 = opendir((path+string("/")+string(pDirent->d_name)).c_str());
    //pDir2 = opendir(path.c_str());
    if (pDir2 == NULL) {
        printf ("IO::loadImages::Cannot open sub-directory '%s'\n", 
          (path+string("/")+string(pDirent->d_name)).c_str());
        continue;
    }
    //Read each file
    while ((pDirent2 = readdir(pDir2)) != NULL) {
      if(string(pDirent2->d_name).find(".")!= string::npos) continue;
      string fullpath = (path+string(pDirent->d_name)+
        string("/")+string(pDirent2->d_name)).c_str();
      //printf ("IO::loadDirectoryOfImages::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      //If new image, set name and path and store
      Image currentImage;
      if(images.find(imageName)==images.end()) {
        printf ("IO::loadImages::Adding new image '%s'\n", imageName.c_str());
        currentImage.setName(imageName);
        currentImage.setPath(path+string(pDirent->d_name));
        images.insert(pair<string,Image>(imageName,currentImage));
      }
      currentImage = images[imageName];
      currentImage.addActivations(fullpath,layerName); 
      images[imageName] = currentImage;
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadImages::Total loaded images: '%lu'\n", images.size());
}


//Loads a directory containing directories of one or more images.
//It builds the features and layers data structures and statistics
void IO::loadLayers(string path, map<string,CNNLayer> &layers){
  struct dirent *pDirent;
  DIR *pDir;
  pDir = opendir(path.c_str());
  if (pDir == NULL) {
    printf ("IO::loadLayers::Cannot open directory '%s'\n", path.c_str());
    return;
  }
  //For each directory in the input path
  while ((pDirent = readdir(pDir)) != NULL) {
    if(string(pDirent->d_name).find(".")!= string::npos) continue;
    struct dirent *pDirent2;
    DIR *pDir2;
    pDir2 = opendir((path+string("/")+string(pDirent->d_name)).c_str());
    //pDir2 = opendir(path.c_str());
    if (pDir2 == NULL) {
        printf ("IO::loadLayers::Cannot open sub-directory '%s'\n", 
          (path+string("/")+string(pDirent->d_name)).c_str());
        continue;
    }
    //Read each file
    while ((pDirent2 = readdir(pDir2)) != NULL) {
      if(string(pDirent2->d_name).find(".")!= string::npos) continue;
      string fullpath = (path+string(pDirent->d_name)+
        string("/")+string(pDirent2->d_name)).c_str();
      //printf ("IO::loadDirectoryOfImages::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      //If new layer, set name and store
      CNNLayer currentLayer;
      if(layers.find(layerName)==layers.end()){
        printf ("IO::loadLayers::Adding new layer '%s'\n", layerName.c_str());
        currentLayer.setName(layerName);
        layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
      }
      currentLayer = layers[layerName];
      currentLayer.addFeatures(fullpath);
      layers[layerName] = currentLayer;
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadLayers::Total loaded layers: '%lu'\n", layers.size());
  //Once all layers have been loaded, compute their statistics
  for(map<string,CNNLayer>::iterator it=layers.begin();it!=layers.end();it++){
    it->second.computeLayerStatistics();
    printf ("IO::loadLayers::Done computing statistics of layer %s\n",it->second.getName().c_str());
  }
  printf ("IO::loadLayers::Done computing all layer statistics\n");
}
