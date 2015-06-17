#include <dirent.h>
#include <cstring>
#include <map>
#include <set>
#include <fstream>
#include <istream>
#include <iostream>

#include "../include/IO.hpp"
#include "../include/Image.hpp"
#include "../include/CNNLayer.hpp"

using std::map;
using std::set;
using std::ofstream;
using std::ifstream;
using std::ios;

//Reads a directory containing sub-directories/images,
// and loads the data of each sub-directory/image.
//Feature activation data is stored per layers/files,
// each layer/file containing lists of features.
//Features are lines of comma separated activation values
// and feature identifiers.
//It also computes a few statistics for the values of each feature
void IO::loadImagesAndLayersFromTXTFile(string path, map<string,Image> &images, map<string,CNNLayer> &layers){
  struct dirent *pDirent;
  DIR *pDir;
  pDir = opendir(path.c_str());
  if (pDir == NULL) {
    printf ("IO::loadImagesAndLayersFromTXTFile::Cannot open directory '%s'\n", path.c_str());
    return;
  }
  set<string> images_loaded;
  //For each directory in the input path
  while ((pDirent = readdir(pDir)) != NULL) {
    if(string(pDirent->d_name).find(".")!= string::npos) continue;
    struct dirent *pDirent2;
    DIR *pDir2;
    pDir2 = opendir((path+string("/")+string(pDirent->d_name)).c_str());
    if (pDir2 == NULL) {
        printf ("IO::loadImagesAndLayersFromTXTFile::Cannot open sub-directory '%s'\n", 
          (path+string("/")+string(pDirent->d_name)).c_str());
        continue;
    }
    //Read each file
    while ((pDirent2 = readdir(pDir2)) != NULL) {
      if(string(pDirent2->d_name).find(".")!= string::npos) continue;
      string fullpath = (path+string(pDirent->d_name)+
        string("/")+string(pDirent2->d_name)).c_str();
      //printf ("IO::loadImagesAndLayersFromTXTFile::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      if(images_loaded.find(imageName)==images_loaded.end()){
        images_loaded.insert(imageName);
        printf ("IO::loadImagesAndLayersFromTXTFile::Processing file of new image '%s' number %u\n", imageName.c_str(),images_loaded.size());
      }
      //If new layer, set name and store
      CNNLayer &currentLayer = layers[layerName];
      if(currentLayer.getFeatures().size()==0){
        printf ("IO::loadImagesAndLayersFromTXTFile::Adding new layer '%s'\n", layerName.c_str());
        currentLayer.setName(layerName);
        layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
      }
      currentLayer.addFeatures(fullpath);
      //If new image, set name and path and store
      Image &currentImage = images[imageName];
      if(currentImage.getActivations().size()==0) {
        printf ("IO::loadImagesAndLayersFromTXTFile::Adding new image '%s'\n", imageName.c_str());
        currentImage.setName(imageName);
        currentImage.setPath(path+string(pDirent->d_name));
        images.insert(pair<string,Image>(imageName,currentImage));
      }
      currentImage.addActivations(fullpath,layerName); 
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadImagesAndLayersFromTXTFile::Total loaded images: '%u'\n", images.size());
  //Once all images have been loaded, compute the layer statistics
  for(map<string,CNNLayer>::iterator it=layers.begin();it!=layers.end();it++){
    it->second.computeLayerStatistics();
    //printf ("IO::loadImagesAndLayersFromTXTFile::Done computing statistics of layer %s\n",it->second.getName().c_str());
  }
  printf ("IO::loadImagesAndLayersFromTXTFile::Done computing all layer statistics\n");
}


//Loads a directory containing directories of one or more images.
//It builds the images data structures and statistics
void IO::loadImagesFromTXTFile(string path, map<string,Image> &images){
  struct dirent *pDirent;
  DIR *pDir;
  pDir = opendir(path.c_str());
  if (pDir == NULL) {
    printf ("IO::loadImagesFromTXTFile::Cannot open directory '%s'\n", path.c_str());
    return;
  }
  set<string> images_loaded;
  //For each directory in the input path
  while ((pDirent = readdir(pDir)) != NULL) {
    if(string(pDirent->d_name).find(".")!= string::npos) continue;
    struct dirent *pDirent2;
    DIR *pDir2;
    pDir2 = opendir((path+string("/")+string(pDirent->d_name)).c_str());
    if (pDir2 == NULL) {
        printf ("IO::loadImagesFromTXTFile::Cannot open sub-directory '%s'\n", 
          (path+string("/")+string(pDirent->d_name)).c_str());
        continue;
    }
    //Read each file
    while ((pDirent2 = readdir(pDir2)) != NULL) {
      if(string(pDirent2->d_name).find(".")!= string::npos) continue;
      string fullpath = (path+string(pDirent->d_name)+
        string("/")+string(pDirent2->d_name)).c_str();
      //printf ("IO::loadImagesFromTXTFile::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      if(images_loaded.find(imageName)==images_loaded.end()){
        images_loaded.insert(imageName);
        printf ("IO::loadImagesFromTXTFile::Processing file of new image '%s' number %u\n", imageName.c_str(),images_loaded.size());
      }
      //If new image, set name and path and store
      Image &currentImage = images[imageName];
      if(currentImage.getActivations().size()==0) {
        printf ("IO::loadImagesFromTXTFile::Adding new image '%s'\n", imageName.c_str());
        currentImage.setName(imageName);
        currentImage.setPath(path+string(pDirent->d_name));
        images.insert(pair<string,Image>(imageName,currentImage));
      }
      currentImage.addActivations(fullpath,layerName); 
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadImagesFromTXTFile::Total loaded images: '%u'\n", images.size());
}


//Loads a directory containing directories of one or more images.
//It builds the features and layers data structures and statistics
void IO::loadLayersFromTXTFile(string path, map<string,CNNLayer> &layers){
  struct dirent *pDirent;
  DIR *pDir;
  pDir = opendir(path.c_str());
  if (pDir == NULL) {
    printf ("IO::loadLayersFromTXTFile::Cannot open directory '%s'\n", path.c_str());
    return;
  }
  set<string> images_loaded;
  //For each directory in the input path
  while ((pDirent = readdir(pDir)) != NULL) {
    if(string(pDirent->d_name).find(".")!= string::npos) continue;
    struct dirent *pDirent2;
    DIR *pDir2;
    pDir2 = opendir((path+string("/")+string(pDirent->d_name)).c_str());
    if (pDir2 == NULL) {
        printf ("IO::loadLayersFromTXTFile::Cannot open sub-directory '%s'\n", 
          (path+string("/")+string(pDirent->d_name)).c_str());
        continue;
    }
    //Read each file
    while ((pDirent2 = readdir(pDir2)) != NULL) {
      if(string(pDirent2->d_name).find(".")!= string::npos) continue;
      string fullpath = (path+string(pDirent->d_name)+
        string("/")+string(pDirent2->d_name)).c_str();
      //printf ("IO::loadLayersFromTXTFile::Processing feature file '%s'\n", fullpath.c_str());
      string layerName = string(pDirent2->d_name)
        .substr(string(pDirent2->d_name).find_last_of("_")+1);
      string imageName = string(pDirent2->d_name)
        .substr(0,string(pDirent2->d_name).find_last_of("_"));
      if(images_loaded.find(imageName)==images_loaded.end()){
        images_loaded.insert(imageName);
        printf ("IO::loadLayersFromTXTFile::Processing file of new image '%s' number %u\n", imageName.c_str(),images_loaded.size());
      }
      //If new layer, set name and store
      CNNLayer &currentLayer = layers[layerName];
      if(currentLayer.getFeatures().size()==0){
        printf ("IO::loadLayersFromTXTFile::Adding new layer '%s'\n", layerName.c_str());
        currentLayer.setName(layerName);
        layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
      }
      currentLayer.addFeatures(fullpath);
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadLayersFromTXTFile::Total loaded layers: '%u'\n", layers.size());
  //Once all layers have been loaded, compute their statistics
  for(map<string,CNNLayer>::iterator it=layers.begin();it!=layers.end();it++){
    it->second.computeLayerStatistics();
    //printf ("IO::loadLayersFromTXTFile::Done computing statistics of layer %s\n",it->second.getName().c_str());
  }
  printf ("IO::loadLayersFromTXTFile::Done computing all layer statistics\n");
}

//Store in an output file the list of vertices defined by images
string IO::writeImagesVerticesToTXTFile(string filename, const map<string,Image> &images){
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
string IO::writeLayersVerticesToTXTFile(string filename, const map<string,CNNLayer> &layers){
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
string IO::writeImagesAndLayersVerticesToTXTFile(string filename, const map<string,Image> &images, const map<string,CNNLayer> &layers){
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
string IO::writeImagesAndLayersEdgesToTXTFile(string filename, const map<string,Image> &images, const map<string,CNNLayer> &layers){
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

//Store in an output file the features loaded form images
vector<string> IO::writeLayersToTXTFile(string filename, const map<string,CNNLayer> &layers){
  vector<string> filenames;
  //For each layer
  for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
    map<int,CNNFeature> lay = it->second.getFeatures();
    ofstream output_file;
    output_file.open(filename);
    //Each feature is a vertex
    for(map<int,CNNFeature>::iterator it2 = lay.begin(); it2!=lay.end(); it2++){
      output_file<<it2->second.getId()<<" "<<it2->second.getMean()<<" "<<it2->second.getStdDev()<<"\n";  
    }
    output_file.close();
    filenames.push_back(filename);
  }
  return filenames;
}

void IO::writeLayersToBinaryFile(string const filename, map<string,CNNLayer> const layers){
  size_t num_layers = layers.size();
  size_t namesSize[num_layers];
  size_t featuresPerLayer[num_layers];
  //Compute total number of features, features per layer, and layer name lenghts
  int i=0;
  for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
    namesSize[i] = it->first.size();
    featuresPerLayer[i] = it->second.getFeatures().size();
    i++;
  }
  ofstream fout( filename.c_str(), ios::out | ios::binary );
  if (fout.is_open()) {
    //Write num layers, num values, values per layer, and sizes of layer names
    fout.write( (char*)&num_layers, sizeof(size_t) );
    fout.write( (char*)&namesSize, sizeof(size_t)*num_layers );
    fout.write( (char*)&featuresPerLayer, sizeof(size_t)*num_layers );
    //For each layer
    map<string,CNNLayer>::const_iterator it = layers.begin();
    for ( size_t ilayer = 0; ilayer < num_layers; ilayer += 1 ) {
      //TODO: ERROR HANDLING if(it==layers.end())
      //Write layer name
      string tmp_str = it->first;
      fout.write( (char*)&(tmp_str), sizeof(char)*namesSize[ilayer] );
      //For each feature
      map<int,CNNFeature>::iterator it2 = it->second.getFeatures().begin();
      for( size_t ifeat = 0; ifeat < featuresPerLayer[ilayer]; ifeat++){
        //TODO: ERROR HANDLING if (it2==it->second.getFeatures().end())
        //Write feature Id, mean, stdDev and activationThreshold
        int tmp_int = it2->second.getId();
        fout.write( (char*)(&tmp_int), sizeof(size_t) );
        float tmp_float = it2->second.getMean();
        fout.write( reinterpret_cast<char*>(&tmp_float), sizeof(float) );
        tmp_float = it2->second.getStdDev();
        fout.write( reinterpret_cast<char*>(&tmp_float), sizeof(float) );
        tmp_float = it2->second.getActivationThreshold();
        fout.write( reinterpret_cast<char*>(&tmp_float), sizeof(float) );
        it2++;
      } 
      it++;
    }
  }
}

void IO::loadLayersFromBinaryFile(string const filename, map<string,CNNLayer> &layers){
  ifstream fin( filename.c_str(), ios::out | ios::binary  );
  if (fin.is_open()) {
    int num_layers;
    size_t namesSize[num_layers];
    size_t featuresPerLayer[num_layers];
    fin.read((char*) &num_layers, sizeof(size_t) );
    fin.read((char*) &namesSize, sizeof(size_t)*num_layers );
    fin.read((char*) &featuresPerLayer, sizeof(size_t)*num_layers );
    //For each layer
    CNNLayer currentLayer;
    string currentName;
    for ( size_t ilayer = 0; ilayer < num_layers; ilayer += 1 ) {
      char* tmp_str = new char[namesSize[ilayer]+1];
      fin.read( (char*)&tmp_str, namesSize[ilayer] );
      tmp_str[namesSize[ilayer]] = '\0';
      currentName = tmp_str;
      //For each feature
      for( size_t ifeat = 0; ifeat < featuresPerLayer[ilayer]; ifeat++){
        CNNFeature currentFeature;
        int currentId;
        //Write feature Id, mean, stdDev and activationThreshold
        fin.read( (char*)(&currentId), sizeof(size_t) );
        float tmp_float;
        fin.read( reinterpret_cast<char*>(&tmp_float), sizeof(float) );
        currentFeature.setMean(tmp_float);
        fin.read( reinterpret_cast<char*>(&tmp_float), sizeof(float) );
        currentFeature.setStdDev(tmp_float);
        fin.read( reinterpret_cast<char*>(&tmp_float), sizeof(float) );
        currentFeature.setActivationThreshold(tmp_float);
        currentLayer.addBasicFeature(pair<int,CNNFeature>(currentId,currentFeature));
      }
      layers.insert(pair<string,CNNLayer>(currentName,currentLayer));
    }
  }
}
