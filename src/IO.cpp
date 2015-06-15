#include <dirent.h>
#include <cstring>
#include <map>
#include <fstream>

#include "../include/IO.hpp"
#include "../include/Image.hpp"
#include "../include/CNNLayer.hpp"
#include "../include/Util.hpp"

using std::map;
using std::ofstream;

//Reads a directory containing sub-directories/images,
// and loads the data of each sub-directory/image.
//Feature activation data is stored per layers/files,
// each layer/file containing lists of features.
//Features are lines of comma separated activation values
// and feature identifiers.
//It also computes a few statistics for the values of each feature
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
      //printf ("IO::loadImagesAndLayers::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      //If new layer, set name and store
      CNNLayer currentLayer;
      if(layers.find(layerName)==layers.end()){
        //printf ("IO::loadImagesAndLayers::Adding new layer '%s'\n", layerName.c_str());
        currentLayer.setName(layerName);
        layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
      }
      currentLayer = layers[layerName];
      currentLayer.addFeatures(fullpath);
      layers[layerName] = currentLayer;
      //If new image, set name and path and store
      Image currentImage;
      if(images.find(imageName)==images.end()) {
        //printf ("IO::loadImagesAndLayers::Adding new image '%s'\n", imageName.c_str());
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
    //printf ("IO::loadImagesAndLayers::Done computing statistics of layer %s\n",it->second.getName().c_str());
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
      //printf ("IO::loadImages::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      //If new image, set name and path and store
      Image currentImage;
      if(images.find(imageName)==images.end()) {
        //printf ("IO::loadImages::Adding new image '%s'\n", imageName.c_str());
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
      //printf ("IO::loadLayers::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      //If new layer, set name and store
      CNNLayer currentLayer;
      if(layers.find(layerName)==layers.end()){
        //printf ("IO::loadLayers::Adding new layer '%s'\n", layerName.c_str());
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
    //printf ("IO::loadLayers::Done computing statistics of layer %s\n",it->second.getName().c_str());
  }
  printf ("IO::loadLayers::Done computing all layer statistics\n");
}

//Store in an output file the list of vertices defined by images
string IO::writeImagesVertices(const map<string,Image> &images){
  //Generate a random string to build a unique filename
  char rand_name[5];
  Util::generate_random_string(rand_name,5);
  string filename = "./output/imagesVertices_"+string(rand_name)+".tir";
  ofstream output_file;
  output_file.open(filename);
  //Each image is a vertex
  for(map<string,Image>::const_iterator it = images.begin(); it!=images.end(); it++){
    output_file<<it->second.getName()<<"\n";  
  }
  output_file.close();
  return filename;
}

//Store in an output file the list of vertices defined by layers
string IO::writeLayersVertices(const map<string,CNNLayer> &layers){
  //Generate a random string to build a unique filename
  char rand_name[5];
  Util::generate_random_string(rand_name,5);
  string filename = "./output/layersVertices_"+string(rand_name)+".tir";
  ofstream output_file;
  output_file.open(filename);
  //For each layer
  for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
    map<int,CNNFeature> lay = it->second.getFeatures();
    //Each feature is a vertex
    for(map<int,CNNFeature>::iterator it2 = lay.begin(); it2!=lay.end(); it2++){
      output_file<<it->second.getName()<<"_"<<it2->second.getId()<<"\n";  
    }
  }
  output_file.close();
  return filename;
}

//Store in the same output file the list of vertices defined by layers and images
string IO::writeImagesAndLayersVertices(const map<string,Image> &images, const map<string,CNNLayer> &layers){
  //Generate a random string to build a unique filename
  char rand_name[5];
  Util::generate_random_string(rand_name,5);
  string filename = "./output/images-layersVertices_"+string(rand_name)+".tir";
  ofstream output_file;
  output_file.open(filename);
  //Each image is a vertex
  for(map<string,Image>::const_iterator it = images.begin(); it!=images.end(); it++){
    output_file<<it->second.getName()<<"\n";  
  }
  //For each layer
  for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
    map<int,CNNFeature> lay = it->second.getFeatures();
    //Each feature is a vertex
    for(map<int,CNNFeature>::iterator it2 = lay.begin(); it2!=lay.end(); it2++){
      output_file<<it->second.getName()<<"_"<<it2->second.getId()<<"\n";  
    }
  }
  output_file.close();
  return filename;
}

//Store in an output file the list of edges defined by layers and images
string IO::writeImagesAndLayersEdges(const map<string,Image> &images, const map<string,CNNLayer> &layers){
  //Generate a random string to build a unique filename
  char rand_name[5];
  Util::generate_random_string(rand_name,5);
  string filename = "./output/imagesEdges_"+string(rand_name)+".tir";
  ofstream output_file;
  output_file.open(filename);
  //For each image
  for(map<string,Image>::const_iterator it = images.begin(); it!=images.end(); it++){
    map<string,map<int,float> > lays = it->second.getRelevantFeatures();
    string imageVertexName = it->second.getName();
    //For each layer
    for(map<string,map<int,float> >::const_iterator it2 = lays.begin();it2!=lays.end();it2++){
      map<int,float> feat = it2->second;
      //For each relevant feature
      for(map<int,float>::const_iterator it3 = feat.begin(); it3!=feat.end(); it3++){
        output_file<<imageVertexName<<" "<<it2->first<<"_"<<it3->first <<"\n";  
      }
    }
  }
  output_file.close();
  return filename;
}
