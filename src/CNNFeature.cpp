#include <cmath>

#include "../include/CNNFeature.hpp"

void CNNFeature::computeMean(){
    mean = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      mean=mean+*i;
    }
    if(mean!=0) mean = mean/numValues;
}

void CNNFeature::computeAbsDev(){
    absoluteDev = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      absoluteDev=absoluteDev+abs(*i-mean);
    }
    //Add the deviation of the null values
    for(int i =values.size(); i<=numValues; i++){
      absoluteDev=absoluteDev+mean;
    }
    if(absoluteDev!=0) absoluteDev = absoluteDev/numValues;
}

void CNNFeature::computeStdDev(){
    standardDev = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      standardDev=standardDev+((*i-mean)*(*i-mean));
    }
    //Add the deviation of the null values
    for(int i = values.size(); i<numValues ; i++){
      standardDev=standardDev+((mean)*(mean)); 
    }
    if(standardDev!=0) standardDev = sqrt(standardDev/(numValues-1));
}

void CNNFeature::computeStatistics(){
    computeMean();
    computeAbsDev();
    computeStdDev();
    computeActivationThreshold();
}

void CNNFeature::computeActivationThreshold(){
    activationThreshold = mean + (3.0*standardDev);
}

