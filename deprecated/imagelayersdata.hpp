#ifndef TIRAMISU_HPP_
#define TIRAMISU_HPP_

#include <map>
#include <string>

namespace tiramisu {

using std::map;
using std::string;

class ImageLayersData {
  public:
    void loadLayersFromFile(const string inputFile, const string type);
    inline map<string,map<int,float> >& getLayersData() { return layersData; }
    inline void setLayersData(map<string,map<int,float> > val) { layersData = val;}

  protected:
    map<string,map<int,float> > layersData;

}; //  ImageLayersData

} //  namespace tiramisu

#endif  // TIRAMISU_HPP_
