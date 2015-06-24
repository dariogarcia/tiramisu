#include <dirent.h>
#include <cstring>
#include <map>
#include <set>
#include <fstream>
#include <istream>
#include <iostream>
#include <sstream>
#include <iterator>

#include "../include/IO.hpp"
#include "../include/Image.hpp"
#include "../include/CNNLayer.hpp"

using std::map;
using std::set;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::istringstream;
using std::istream_iterator;
using std::back_inserter;

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
        printf ("IO::loadImagesAndLayersFromTXTFile::Processing file of new image '%s' number %u\n", imageName.c_str(),(unsigned int)images_loaded.size());
      }
      //If new layer, set name and store
      CNNLayer &currentLayer = layers[layerName];
      if(currentLayer.getFeaturesConst().size()==0){
        printf ("IO::loadImagesAndLayersFromTXTFile::Adding new layer '%s'\n", layerName.c_str());
        currentLayer.setName(layerName);
        layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
      }
      currentLayer.addFeatures(fullpath);
      //If new image, set name and path and store
      Image &currentImage = images[imageName];
      if(currentImage.getActivationsDyn().size()==0) {
        printf ("IO::loadImagesAndLayersFromTXTFile::Adding new image '%s'\n", imageName.c_str());
        currentImage.setImageName(imageName);
        currentImage.setPath(path+string(pDirent->d_name));
        images.insert(pair<string,Image>(imageName,currentImage));
      }
      currentImage.addActivations(fullpath,layerName); 
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadImagesAndLayersFromTXTFile::Total loaded images: '%u'\n", (unsigned int)images.size());
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
        printf ("IO::loadImagesFromTXTFile::Processing file of new image '%s' number %u\n", imageName.c_str(),(unsigned int)images_loaded.size());
      }
      //If new image, set name and path and store
      Image &currentImage = images[imageName];
      if(currentImage.getActivationsDyn().size()==0) {
        printf ("IO::loadImagesFromTXTFile::Adding new image '%s'\n", imageName.c_str());
        currentImage.setImageName(imageName);
        currentImage.setPath(path+string(pDirent->d_name));
        images.insert(pair<string,Image>(imageName,currentImage));
      }
      currentImage.addActivations(fullpath,layerName); 
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadImagesFromTXTFile::Total loaded images: '%u'\n", (unsigned int)images.size());
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
        printf ("IO::loadLayersFromTXTFile::Processing file of new image '%s' number %u\n", imageName.c_str(),(unsigned int)images_loaded.size());
      }
      //If new layer, set name and store
      CNNLayer &currentLayer = layers[layerName];
      if(currentLayer.getFeaturesConst().size()==0){
        printf ("IO::loadLayersFromTXTFile::Adding new layer '%s'\n", layerName.c_str());
        currentLayer.setName(layerName);
        layers.insert(pair<string,CNNLayer>(layerName,currentLayer));
      }
      currentLayer.addFeatures(fullpath);
    }
    closedir(pDir2);
  }
  closedir (pDir);
  printf ("IO::loadLayersFromTXTFile::Total loaded layers: '%u'\n", (unsigned int)layers.size());
  //Once all layers have been loaded, compute their statistics
  for(map<string,CNNLayer>::iterator it=layers.begin();it!=layers.end();it++){
    it->second.computeLayerStatistics();
    //printf ("IO::loadLayersFromTXTFile::Done computing statistics of layer %s\n",it->second.getName().c_str());
  }
  printf ("IO::loadLayersFromTXTFile::Done computing all layer statistics\n");
}


//Read a file with the image names and image classes, and update image data with them accordingly
void IO::readAndSetImageClasses(string path, map<string,Image> &images){
  ifstream infile(path.c_str());
  string line;
  if(infile.is_open()){
    while(getline(infile,line)){
      vector<std::string> strs;
      istringstream is(line);
      copy(istream_iterator<string>(is),istream_iterator<string>(),back_inserter<vector<string> >(strs));
      string image_name = strs[0];
      string image_class_name = strs[1];
      //Find the image in the map and update its class name
      images[image_name].setClassName(image_class_name);
    }
    infile.close();
  }
  else printf("IO::readImageClasses::ERROR Unable to open file%s\n",path.c_str());
}


//Store in an output file the list of vertices defined by images
string IO::writeImagesVerticesToTXTFile(string filename, const map<string,Image> &images){
  ofstream output_file;
  output_file.open(filename);
  //Each image is a vertex
  for(map<string,Image>::const_iterator it = images.begin(); it!=images.end(); it++){
    output_file<<it->second.getImageName()<<"\n";  
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
    const map<int,CNNFeature> &lay = it->second.getFeaturesConst();
    //Each feature is a vertex
    for(map<int,CNNFeature>::const_iterator it2 = lay.begin(); it2!=lay.end(); it2++){
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
    output_file<<it->second.getImageName()<<"\n";  
  }
  //For each layer
  for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
    const map<int,CNNFeature> &lay = it->second.getFeaturesConst();
    //Each feature is a vertex
    for(map<int,CNNFeature>::const_iterator it2 = lay.begin(); it2!=lay.end(); it2++){
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
    const map<string,map<int,float> > &lays = it->second.getRelevantFeaturesConst();
    string imageVertexName = it->second.getImageName();
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
    const map<int,CNNFeature> &lay = it->second.getFeaturesConst();
    ofstream output_file;
    output_file.open(filename);
    //Each feature is a vertex
    for(map<int,CNNFeature>::const_iterator it2 = lay.begin(); it2!=lay.end(); it2++){
      output_file<<it2->second.getId()<<" "<<it2->second.getMean()<<" "<<it2->second.getStdDev()<<"\n";  
    }
    output_file.close();
    filenames.push_back(filename);
  }
  return filenames;
}

void IO::writeLayersToBinaryFile(string const filename, map<string,CNNLayer> const &layers){
  size_t num_layers = layers.size();
  size_t namesSize[num_layers];
  size_t featuresPerLayer[num_layers];
  //Compute features per layer, and layer name lenghts
  int i=0;
  for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
    namesSize[i] = it->first.size();
    featuresPerLayer[i] = it->second.getFeaturesConst().size();
    i++;
  }
  ofstream fout( filename.c_str(), ios::out | ios::binary );
  if (fout.is_open()) {
    //Write num layers, num values, values per layer, and sizes of layer names
    fout.write( (char*)&num_layers, sizeof(size_t) );
    fout.write( (char*)&namesSize, sizeof(size_t)*num_layers );
    fout.write( (char*)&featuresPerLayer, sizeof(size_t)*num_layers );
    //For each layer
    size_t ilayer = 0;
    for(map<string,CNNLayer>::const_iterator it = layers.begin(); it!=layers.end(); it++){
      const map<int,CNNFeature> &feats =  it->second.getFeaturesConst();
      //Write layer name
      char* tmp_str = new char[namesSize[ilayer]+1];
      //tmp_str = it->first.c_str();
      std::copy(it->first.begin(), it->first.end(), tmp_str);
      tmp_str[namesSize[ilayer]] = '\0'; // don't forget the terminating 0

      fout.write( (char*)tmp_str, sizeof(char)*namesSize[ilayer]+1 );
      //For each feature
      int size = feats.size();
      char* buffer;
      buffer = (char*) malloc ((sizeof(float)*3+sizeof(int))*size);
      int current_size=0;
      for(map<int,CNNFeature>::const_iterator it2 = feats.begin(); it2 != feats.end(); it2++){
        //Write feature Id, mean, stdDev and activationThreshold
        *((size_t *)&buffer[current_size]) = it2->second.getId();
        current_size += sizeof(int);
        *((float *)&buffer[current_size]) = it2->second.getMean();
        current_size += sizeof(float);
        *((float *)&buffer[current_size]) = it2->second.getStdDev();
        current_size += sizeof(float);
        *((float *)&buffer[current_size]) = it2->second.getActivationThreshold();
        current_size += sizeof(float);
      }
      int idzero = *((int *)&buffer[0]);
      float floatzero = *((float*)&buffer[(sizeof(int))]);
      float floatone = *((float*)&buffer[(sizeof(int))+sizeof(float)]);
      fout.write( (char*)(buffer), sizeof(float)*3*size + sizeof(int)*size );
      ilayer++;
    }
    fout.close();
  }
  else printf ("IO::writeLayersToBinaryFile::Cant open file to write %s\n",filename.c_str());
}

void IO::loadLayersFromBinaryFile(string const filename, map<string,CNNLayer> &layers){
  ifstream fin( filename.c_str(), ios::out | ios::binary  );
  if (fin.is_open()) {
    int num_layers;
    fin.read((char*) &num_layers, sizeof(size_t) );
    size_t namesSize[num_layers];
    size_t featuresPerLayer[num_layers];
    fin.read((char*) &namesSize, sizeof(size_t)*num_layers );
    fin.read((char*) &featuresPerLayer, sizeof(size_t)*num_layers );
    //For each layer
    for ( int i = 0; i < num_layers; i++) {
      CNNLayer currentLayer;
      char* tmp_str = new char[namesSize[i]+1];
      fin.read( tmp_str, namesSize[i]+1 );
      tmp_str[namesSize[i]+1] = '\0';
      string currentName (tmp_str);
      //For each feature
      for(int j = 0; j < featuresPerLayer[i]; j++){
        //Read feature Id, mean, stdDev and activationThreshold
        CNNFeature currentFeature;
        char * reader = (char*) malloc (sizeof(int));
        fin.read( reader, sizeof(int) );
        currentFeature.setId(*(int*)reader);
        char * reader_float = (char*) malloc (sizeof(float));
        fin.read( reader_float, sizeof(float) );
        currentFeature.setMean(*(float*)reader_float);
        char * reader_float2 = (char*) malloc (sizeof(float));
        fin.read( reader_float2, sizeof(float) );
        currentFeature.setStdDev( *(float*)reader_float2);
        char * reader_float3 = (char*) malloc (sizeof(float));
        fin.read( reader_float3, sizeof(float) );
        currentFeature.setActivationThreshold(*(float*)reader_float3);
        currentLayer.addBasicFeature(pair<int,CNNFeature>(*(int*)reader,currentFeature));
      }
      //printf("loaded layer %s\n",currentName.c_str());
      layers.insert(pair<string,CNNLayer>(currentName,currentLayer));
    }
  }
}


//TODO: not validated. To implement reader as well
void IO::writeImageClassToBinaryFile(string const filename, ImageClass const &imageC){
  //Compute self name size
  size_t imageClass_name_size = imageC.getName().size();
  //Compute image names size
  size_t num_images_names = imageC.getImageNames().size();
  size_t image_name_size[num_images_names];
  int nameIDX = 0;
  for(vector<string>::const_iterator it = imageC.getImageNames().begin(); it!=imageC.getImageNames().end(); it++){
    image_name_size[nameIDX] = (*it).size();
    nameIDX++;
  }
  //Compute num layers and layers name size
  size_t num_layers = imageC.getMeanActivations().size();
  size_t layer_namesSize[num_layers];
  size_t featuresPerLayer[num_layers];
  //Compute features per layer, and layer name lenghts
  int i=0;
  for(map<string,map<int,float> >::const_iterator it = imageC.getMeanActivations().begin(); it!=imageC.getMeanActivations().end(); it++){
    layer_namesSize[i] = it->first.size();
    featuresPerLayer[i] = it->second.size();
    i++;
  }
  ofstream fout( filename.c_str(), ios::out | ios::binary );
  if (fout.is_open()) {
    //Write self name size and name,
    fout.write( (char*)&imageClass_name_size, sizeof(size_t));
    char * name = new char[imageClass_name_size+1];
    std::copy(imageC.getName().begin(), imageC.getName().end(), name);
    name[imageClass_name_size] = '\0';
    fout.write( (char*)name, sizeof(char)*imageClass_name_size+1);
    //Write num images, image names size and image names
    fout.write( (char*)&num_images_names, sizeof(size_t));
    fout.write( (char*)&image_name_size, sizeof(size_t)*num_images_names);
    size_t iImage = 0; 
    for(vector<string>::const_iterator it = imageC.getImageNames().begin(); it!=imageC.getImageNames().end(); it++){
      char * img_name = new char[image_name_size[iImage]+1];
      std::copy((*it).begin(), (*it).end(), img_name);
      img_name[image_name_size[iImage]] = '\0';
      fout.write( (char*)img_name, sizeof(char)*image_name_size[iImage]+1);
      iImage++;
    }
    //Write num layers, num values, values per layer, and sizes of layer names
    fout.write( (char*)&num_layers, sizeof(size_t) );
    fout.write( (char*)&layer_namesSize, sizeof(size_t)*num_layers );
    fout.write( (char*)&featuresPerLayer, sizeof(size_t)*num_layers );
    //For each layer
    size_t ilayer = 0;
    for(map<string,map<int,float> >::const_iterator it = imageC.getMeanActivations().begin(); it!=imageC.getMeanActivations().end(); it++){
      const map<int,float> &feats =  it->second;
      //Write layer name
      char* tmp_str = new char[layer_namesSize[ilayer]+1];
      std::copy(it->first.begin(), it->first.end(), tmp_str);
      tmp_str[layer_namesSize[ilayer]] = '\0'; // don't forget the terminating 0
      fout.write( (char*)tmp_str, sizeof(char)*layer_namesSize[ilayer]+1 );
      //For each feature
      int size = feats.size();
      char* buffer;
      buffer = (char*) malloc ((sizeof(int)+sizeof(float))*size);
      int current_size=0;
      for(map<int,float>::const_iterator it2 = feats.begin(); it2 != feats.end(); it2++){
        //Write feature Id, mean, stdDev and activationThreshold
        *((size_t *)&buffer[current_size]) = it2->first;
        current_size += sizeof(int);
        *((float *)&buffer[current_size]) = it2->second;
        current_size += sizeof(float);
      }
      fout.write( (char*)(buffer), sizeof(int)*size + sizeof(float)*size );
      ilayer++;
    }
    fout.close();
  }
  else printf ("IO::writeImageClassToBinaryFile::Cant open file to write %s\n",filename.c_str());
}

