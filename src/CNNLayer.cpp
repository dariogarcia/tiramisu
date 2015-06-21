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
      CNNFeature &feat = features[key];
      //If its the first time we found this feature, initialize
      if(feat.getValues().size()==0){
        feat.setId(key);
        feat.setMean(-1);
        feat.setStdDev(-1);
        feat.setAbsDev(-1);
        feat.setNumValues(1);
        feat.addValue(value);
      }
      else {
        if(value!=0) feat.addValue(value);
        feat.increaseNumValues();
      }
    }
    infile.close();
  }
  else printf("CNNLayer::addFeatures::ERROR Unable to open file%s\n",path.c_str());
}

void CNNLayer::addBasicFeature(const pair<int,CNNFeature> &feat){
  features.insert(feat);
}

//Method to explore and print contents of a set of CNNLayers
//Intended for validation of loading process
void CNNLayer::exploreCNNLayer(){
  printf("-Layer with name '%s', has '%u' activations \n", name.c_str(), features.size());
  int featureCounter = 0;
  for(map<int,CNNFeature>::iterator it2 = features.begin(); it2!=features.end(); it2++){
    CNNFeature currentFeature = it2->second;
    printf("--Feature num: '%d', with Id '%d':'%d', has mean '%f', stdDev '%f', absDev '%f' and '%u' values\n",
      featureCounter,currentFeature.getId(),it2->first,currentFeature.getMean(),
      currentFeature.getStdDev(),currentFeature.getAbsDev(),currentFeature.getValues().size());
    vector<float> values = currentFeature.getValues();
    int valueCounter = 0;
    for(vector<float>::iterator it3 = values.begin(); it3!=values.end(); it3++){
      printf("---Value num: '%d' is '%f'\n",valueCounter,*it3);
      valueCounter++;
    }
    featureCounter++;
  }
}

void CNNLayer::computeLayerStatistics(){
  for(map<int,CNNFeature>::iterator it=features.begin();it!=features.end(); it++){
    CNNFeature updatedFeat = it->second;
    updatedFeat.computeStatistics();
    features[updatedFeat.getId()] = updatedFeat;
  }
}
