#include <fstream>
#include <sstream>
#include <iterator>
#include "../include/CNNLayer.hpp"

using std::ifstream;
using std::istringstream;
using std::istream_iterator;
using std::back_inserter;

void CNNLayer::addFeatures(string path){
  ifstream infile(path.c_str());
  string line;
  if(infile.is_open()){
    map<int,float> values;
    while(getline(infile,line)){
      vector<std::string> strs;
      istringstream is(line);
      copy(istream_iterator<string>(is),istream_iterator<string>(),back_inserter<vector<string> >(strs));
      int key = stoi(strs[1]);
      float value = stof(strs[0]);
      CNNFeature feat = features[key];
      feat.setId(key);
      if(feat.getValues().size()==0){
        feat.setMean(-1);
        feat.setDev(-1);
      }
      feat.addValue(value);
      features[key]=feat;
    }
    infile.close();
  }
  else printf("CNNLayer::addFeatures:: Unable to open file\n");

}
