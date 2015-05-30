#include <dirent.h>
#include <cstring>
#include <map>

#include "../include/Image.hpp"
#include "../include/CNNLayer.hpp"

using std::map;
using std::pair;

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

  for(map<string,CNNLayer>::iterator it=layers.begin();it!=layers.end();it++){
    it->second.computeLayerStatistics();
  }
}


