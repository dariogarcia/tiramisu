#include <iterator>
#include "../include/CNNScheme.hpp"

//Initialize the vectors of a CNNFeature according to the sizes of a CNNScheme
void CNNScheme::printScheme(){
  printf("Printing information of CNN Scheme with %u layers \n",(unsigned int)this->numLayers);
  for(int i = 0 ; i < this->numLayers ; i++){
    printf("-Layer with index %u and name %s has %u features\n",(unsigned int)i, this->layerIdx[i].c_str(), (unsigned int)(this->layerSize[i]));
  }
}
