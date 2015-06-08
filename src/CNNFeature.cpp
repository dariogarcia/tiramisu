#include <cmath>

#include "../include/CNNFeature.hpp"

void CNNFeature::computeMean(){
    mean = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      mean=mean+*i;
    }
    mean = mean/values.size();
}

void CNNFeature::computeAbsDev(){
    absoluteDev = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      absoluteDev=absoluteDev+abs(*i-mean);
    }
    absoluteDev = absoluteDev/values.size();
}

void CNNFeature::computeStdDev(){
    standardDev = 0;
    for(vector<float>::iterator i=values.begin();i!=values.end();i++) {
      standardDev=standardDev+((*i-mean)*(*i-mean));
    }
    standardDev = sqrt(standardDev/values.size());
}

void CNNFeature::computeStatistics(){
    computeMean();
    computeAbsDev();
    computeStdDev();
    computeActivationThreshold();
}

void CNNFeature::computeActivationThreshold(){
    activationThreshold = mean + (3*standardDev);
}
