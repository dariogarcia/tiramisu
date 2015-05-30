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
  data = IO::loadDirectoryOfImages(argv[1]);

  for(map<string,Image>::iterator it = data.first.begin();it!=data.first.end();it++){
    it++;
    it->second.exploreImage();
  }
}  

