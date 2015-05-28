#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>

#include "../include/imagelayersdata.hpp"

namespace tiramisu {

using std::string;
using std::ifstream;
using std::vector;
using std::pair;
using std::istringstream;
using std::istream_iterator;
using std::back_inserter;
using std::cout;
using std::endl;
using std::stoi;
using std::stof;

void ImageLayersData::loadLayersFromFile(string inputFile, string type){
  ifstream infile(inputFile.c_str());
  string line;
  if(infile.is_open()){
    map<int,float> values;
    while(getline(infile,line)){
      vector<std::string> strs;
      istringstream is(line);
      copy(istream_iterator<string>(is),istream_iterator<string>(),back_inserter<vector<string> >(strs));
      int key = stoi(strs[1]);
      float value = stof(strs[0]);
      values.insert(pair<int,float>(key,value));
    }
    layersData.insert(pair<string,map<int,float> > (type,values));
    infile.close();
  }
  else cout<<"Unable to open file"<<endl;

}

} // namespace tiramisu
