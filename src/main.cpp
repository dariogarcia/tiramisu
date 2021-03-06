#include <dirent.h>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>

#include "../include/IO.hpp"
#include "../include/Util.hpp"
#include "../include/CNNScheme.hpp"
#include "../include/ImageClass.hpp"

using std::set;
using std::vector;
using std::pair;

//An arithmetic ImageClass (.first), result of operating on some imageClasses (.second)
typedef pair<ImageClass,set<int> > ArithmeticIC;

void substract(ArithmeticIC &result, string id_first, string id_second, CNNScheme & scheme, vector<ImageClass> & imageClasses){
  ImageClass first;
  ImageClass second;
  for(int i = 0 ; i<imageClasses.size();i++) {
    if(imageClasses[i].getName().compare(id_first)==0) {
      first = imageClasses[i];
      result.second.insert(i);
    }
    if(imageClasses[i].getName().compare(id_second)==0) {
      second = imageClasses[i];
      result.second.insert(i);
    }
  }
  Util::substractImageClass(first,second,result.first,scheme);
  IO::writeImageClassToIVF(first.getName()+".ivf", first,scheme);
  IO::writeImageClassToIVF(second.getName()+".ivf", second,scheme);
  IO::writeImageClassToIVF(result.first.getName()+".ivf", result.first,scheme);
}

void substract(ArithmeticIC &result, string id_first, ArithmeticIC secondIC, CNNScheme & scheme, vector<ImageClass> & imageClasses){
  ImageClass first;
  ImageClass second = secondIC.first;
  for(int i = 0 ; i<imageClasses.size();i++) {
    if(imageClasses[i].getName().compare(id_first)==0) {
      first = imageClasses[i];
      result.second.insert(i);
    }
  }
  Util::substractImageClass(first,second,result.first,scheme);
  IO::writeImageClassToIVF(first.getName()+".ivf", first,scheme);
  IO::writeImageClassToIVF(second.getName()+".ivf", second,scheme);
  IO::writeImageClassToIVF(result.first.getName()+".ivf", result.first,scheme);
}

void add(ArithmeticIC &result, string id_first, string id_second, CNNScheme & scheme, vector<ImageClass> & imageClasses){
  ImageClass first;
  ImageClass second;
  for(int i = 0 ; i<imageClasses.size();i++) {
    if(imageClasses[i].getName().compare(id_first)==0) {
      first = imageClasses[i];
      result.second.insert(i);
    }
    if(imageClasses[i].getName().compare(id_second)==0) {
      second = imageClasses[i];
      result.second.insert(i);
    }
  }
  Util::addImageClass(first,second,result.first,scheme);
  IO::writeImageClassToIVF(first.getName()+".ivf", first,scheme);
  IO::writeImageClassToIVF(second.getName()+".ivf", second,scheme);
  IO::writeImageClassToIVF(result.first.getName()+".ivf", result.first,scheme);
}

pair<ImageClass,double> findClosestArithmeticClass(vector<ImageClass> &imageClasses, ArithmeticIC & arithClass,CNNScheme & scheme, int distanceType){
  vector<ImageClass> tempIC = imageClasses;
  for(int i = arithClass.second.size(); i>0 ;i--) tempIC.erase(tempIC.begin()+(*std::next(arithClass.second.begin(), i-1)));
  pair<ImageClass,double> res = arithClass.first.findClosestClass(tempIC, scheme, distanceType,true);
  printf("ARITH_CLOSEST::Closest class to %s is %s at %f\n",arithClass.first.getName().c_str(),res.first.getName().c_str(),res.second);
  return res;
} 

int main(int argc, char* argv[]){
  CNNScheme scheme;
  CNNFeatures cnnfeatures;
  vector<ImageClass> imageClasses;
  
  time_t t_init;
  time(&t_init);
  IO::loadSchemeFromCFGFile(argv[1], scheme, "");
  time_t t_end;
  time(&t_end);
  double t_diff = difftime (t_end,t_init);
  printf("MAIN::Load scheme took %f\n",t_diff);

  scheme.printScheme();

  //cnnfeatures.initialize(scheme);
  //printf("Done initializing\n");
  //time(&t_init);
  //IO::loadFeaturesFromTXTFile(argv[1], cnnfeatures, scheme);
  //time(&t_end);
  //printf("Load features took %f\n",difftime (t_end,t_init));

  if(true){ 
    vector<Image> images;
    time(&t_init);
    //IO::loadImagesFromTXTFile(argv[1], images, scheme, "");
    IO::loadImagesFromIVFFile(argv[1], images, scheme);
    time(&t_end);
    printf("MAIN::Load images took %f\n",difftime (t_end,t_init));

    //time(&t_init);
    //int normImgType = 1;
    //if(normImgType==1)printf("MAIN::Normalizing Image overall vector\n");
    //if(normImgType==2)printf("MAIN::Normalizing Image vector by CNN layer\n");
    //for(int i = 0 ; i<images.size();i++) images[i].normalizeActivations(normImgType);
    //time(&t_end);
    //printf("Normalizing images took %f\n",difftime (t_end,t_init));
 
    time(&t_init);
    IO::readAndSetImageClasses(argv[2], images);
    time(&t_end);
    printf("MAIN::Read & set image classes took %f\n",difftime (t_end,t_init));
    
    IO::writeImagesOfClassToIVF(images, "n02134084 ice bear, polar bear, Ursus Maritimus, Thalarctos maritimus", scheme);
    IO::writeImagesOfClassToIVF(images, "n02132136 brown bear, bruin, Ursus arctos", scheme);
    IO::writeImagesOfClassToIVF(images, "n02510455 giant panda, panda, panda bear, coon bear, Ailuropoda melanoleuca", scheme);
    IO::writeImagesOfClassToIVF(images, "n03028079 church, church building", scheme);
    IO::writeImagesOfClassToIVF(images, "n03788195 mosque", scheme);
    IO::writeImagesOfClassToIVF(images, "n02825657 bell cote, bell cot", scheme);
    IO::writeImagesOfClassToIVF(images, "n02389026 sorrel", scheme);
    IO::writeImagesOfClassToIVF(images, "n03538406 horse cart, horse-cart", scheme);
    IO::writeImagesOfClassToIVF(images, "n03599486 jinrikisha, ricksha, rickshaw", scheme);

    time(&t_init);
    int meanType = 1;
    if(meanType==1)printf("MAIN::Using arithmetic mean\n");
    if(meanType==2)printf("MAIN::Using harmonic mean\n");
    Util::computeImageClasses(images, scheme, imageClasses, meanType);
    //WARNING IMAGES VECTOR IS DESTROYED
    images.clear();
    //images.~vector<Image>();
    time(&t_end);
    printf("MAIN::Compute image classes took %f\n",difftime (t_end,t_init));
  }

  time(&t_init);
  int normType = 2;
  if(normType==1)printf("MAIN::Normalizing class overall vector\n");
  if(normType==2)printf("MAIN::Normalizing class vector by CNN layer\n");
  for(int i = 0 ; i<imageClasses.size();i++) imageClasses[i].normalizeMeanActivations(normType);
  time(&t_end);
  printf("MAIN::Normalizing image classes took %f\n",difftime (t_end,t_init));


  time(&t_init);
  int distanceType = 2;
  if(distanceType==1)printf("MAIN::Using euclidean distance\n");
  if(distanceType==2)printf("MAIN::Using cosine distance\n");
  for(vector<ImageClass>::iterator it = imageClasses.begin(); it!=imageClasses.end(); it++){
    pair<ImageClass,double> closest = (*it).findClosestClass(imageClasses, scheme, distanceType,true);
    printf("CLOSEST::Closest class to %s is %s at distance %f\n",(*it).getName().c_str(),closest.first.getName().c_str(),closest.second);
  }
  time(&t_end);
  printf("MAIN::Compute image class distances took %f\n",difftime (t_end,t_init));

  time(&t_init);
  vector<ArithmeticIC> ariths;
  ArithmeticIC sorrel_minus_cart;
  substract(sorrel_minus_cart,"n03538406 horse cart, horse-cart", "n02389026 sorrel", scheme, imageClasses);
  ariths.push_back(sorrel_minus_cart);
  //ArithmeticIC whitewolf_minus_timberwolf;
  //substract(whitewolf_minus_timberwolf,"n02114548 white wolf, Arctic wolf, Canis lupus tundrarum", "n02114367 timber wolf, grey wolf, gray wolf, Canis lupus", scheme, imageClasses);
  //ariths.push_back(whitewolf_minus_timberwolf);
  //ArithmeticIC whitewolf_minus_coyote;
  //substract(whitewolf_minus_coyote,"n02114548 white wolf, Arctic wolf, Canis lupus tundrarum", "n02114855 coyote, prairie wolf, brush wolf, Canis latrans", scheme, imageClasses);
  //ariths.push_back(whitewolf_minus_coyote);
  //ArithmeticIC timberwolf_minus_eskimo;
  //substract(timberwolf_minus_eskimo,"n02114367 timber wolf, grey wolf, gray wolf, Canis lupus", "n02109961 Eskimo dog, husky", scheme, imageClasses);
  //ariths.push_back(timberwolf_minus_eskimo);
  //ArithmeticIC whitewolf_minus_eskimo;
  //substract(whitewolf_minus_eskimo,"n02114548 white wolf, Arctic wolf, Canis lupus tundrarum", "n02109961 Eskimo dog, husky", scheme, imageClasses);
  //ariths.push_back(whitewolf_minus_eskimo);
  //ArithmeticIC timberwolf_minus_siberian;
  //substract(timberwolf_minus_siberian,"n02114367 timber wolf, grey wolf, gray wolf, Canis lupus", "n02110185 Siberian husky", scheme, imageClasses);
  //ariths.push_back(timberwolf_minus_siberian);
  //ArithmeticIC whitewolf_minus_siberian;
  //substract(whitewolf_minus_siberian,"n02114548 white wolf, Arctic wolf, Canis lupus tundrarum", "n02110185 Siberian husky", scheme, imageClasses);
  //ariths.push_back(whitewolf_minus_siberian);
  //ArithmeticIC teddy_minus_brownbear;
  //substract(teddy_minus_brownbear,"n04399382 teddy, teddy bear","n02132136 brown bear, bruin, Ursus arctos",scheme, imageClasses);
  //ariths.push_back(teddy_minus_brownbear);
  //ArithmeticIC teddy_minus_blackbear;
  //substract(teddy_minus_blackbear,"n04399382 teddy, teddy bear","n02133161 American black bear, black bear, Ursus americanus, Euarctos americanus",scheme, imageClasses);
  //ariths.push_back(teddy_minus_blackbear);
  //ArithmeticIC slothbear_minus_brownbear;
  //substract(slothbear_minus_brownbear,"n02134418 sloth bear, Melursus ursinus, Ursus ursinus", "n02132136 brown bear, bruin, Ursus arctos", scheme, imageClasses);
  //ariths.push_back(slothbear_minus_brownbear);
  //ArithmeticIC slothbear_minus_blackbear;
  //substract(slothbear_minus_blackbear,"n02134418 sloth bear, Melursus ursinus, Ursus ursinus", "n02133161 American black bear, black bear, Ursus americanus, Euarctos americanus", scheme, imageClasses);
  //ariths.push_back(slothbear_minus_blackbear);
  ArithmeticIC icebear_minus_brownbear;
  substract(icebear_minus_brownbear,"n02134084 ice bear, polar bear, Ursus Maritimus, Thalarctos maritimus", "n02132136 brown bear, bruin, Ursus arctos", scheme, imageClasses);
  ariths.push_back(icebear_minus_brownbear);
  //ArithmeticIC icebear_minus_blackbear;
  //substract(icebear_minus_blackbear,"n02134084 ice bear, polar bear, Ursus Maritimus, Thalarctos maritimus", "n02133161 American black bear, black bear, Ursus americanus, Euarctos americanus", scheme, imageClasses);
  //ariths.push_back(icebear_minus_blackbear);
  ArithmeticIC panda_minus_brownbear;
  substract(panda_minus_brownbear,"n02510455 giant panda, panda, panda bear, coon bear, Ailuropoda melanoleuca", "n02132136 brown bear, bruin, Ursus arctos", scheme, imageClasses);
  ariths.push_back(panda_minus_brownbear);
  //ArithmeticIC panda_minus_blackbear;
  //substract(panda_minus_blackbear,"n02510455 giant panda, panda, panda bear, coon bear, Ailuropoda melanoleuca", "n02133161 American black bear, black bear, Ursus americanus, Euarctos americanus", scheme, imageClasses);
  //ariths.push_back(panda_minus_blackbear);
  //ArithmeticIC icebear_plus_brownbear;
  //add(icebear_plus_brownbear,"n02134084 ice bear, polar bear, Ursus Maritimus, Thalarctos maritimus", "n02132136 brown bear, bruin, Ursus arctos", scheme, imageClasses);
  //ariths.push_back(icebear_plus_brownbear);
  //ArithmeticIC icebear_plus_blackbear;
  //add(icebear_plus_blackbear,"n02134084 ice bear, polar bear, Ursus Maritimus, Thalarctos maritimus", "n02133161 American black bear, black bear, Ursus americanus, Euarctos americanus", scheme, imageClasses);
  //ariths.push_back(icebear_plus_blackbear);
  //ArithmeticIC seacucumber_minus_cucumber;
  //substract(seacucumber_minus_cucumber,"n02321529 sea cucumber, holothurian", "n07718472 cucumber, cuke", scheme, imageClasses);
  //ariths.push_back(seacucumber_minus_cucumber);
  //ArithmeticIC boar_minus_pig;
  //substract(boar_minus_pig,"n02396427 wild boar, boar, Sus scrofa", "n02395406 hog, pig, grunter, squealer, Sus scrofa", scheme, imageClasses);
  //ariths.push_back(boar_minus_pig);
  //ArithmeticIC church_minus_mosque;
  //substract(church_minus_mosque,"n03028079 church, church building", "n03788195 mosque", scheme, imageClasses);
  //ariths.push_back(church_minus_mosque);
  //ArithmeticIC mosque_plus_bell;
  //add(mosque_plus_bell,"n03788195 mosque", "n02825657 bell cote, bell cot", scheme, imageClasses);
  //ariths.push_back(mosque_plus_bell);
  //ArithmeticIC mosque_minus_church;
  //substract(mosque_minus_church,"n03788195 mosque", "n03028079 church, church building", scheme, imageClasses);
  //ariths.push_back(mosque_minus_church);
  //ArithmeticIC computermous_minus_computer;
  //substract(computermous_minus_computer,"n03793489 mouse, computer mouse", "n03180011 desktop computer", scheme, imageClasses);
  //ariths.push_back(computermous_minus_computer);
  //ArithmeticIC computermouse_minus_laptop;
  //substract(computermouse_minus_laptop,"n03793489 mouse, computer mouse", "n03642806 laptop, laptop computer", scheme, imageClasses);
  //ariths.push_back(computermouse_minus_laptop);
  //ArithmeticIC electric_minus_acoustic;
  //substract(electric_minus_acoustic,"n03272010 electric guitar", "n02676566 acoustic guitar", scheme, imageClasses);
  //ariths.push_back(electric_minus_acoustic);
  //ArithmeticIC sportcar_minus_wheel;
  //substract(sportcar_minus_wheel,"n04285008 sports car, sport car", "n02974003 car wheel", scheme, imageClasses);
  //ariths.push_back(sportcar_minus_wheel);
  //ArithmeticIC platypus_minus_marmot;
  //substract(platypus_minus_marmot,"n01873310 platypus, duckbill, duckbilled platypus, duck-billed platypus, Ornithorhynchus anatinus", "n02361337 marmot", scheme, imageClasses);
  //ariths.push_back(platypus_minus_marmot);
  //ArithmeticIC platypus_minus_beaver;
  //substract(platypus_minus_beaver,"n01873310 platypus, duckbill, duckbilled platypus, duck-billed platypus, Ornithorhynchus anatinus", "n02363005 beaver", scheme, imageClasses);
  //ariths.push_back(platypus_minus_beaver);
  //ArithmeticIC platypus_minus_furcoat;
  //substract(platypus_minus_furcoat,"n01873310 platypus, duckbill, duckbilled platypus, duck-billed platypus, Ornithorhynchus anatinus", "n03404251 fur coat", scheme, imageClasses);
  //ariths.push_back(platypus_minus_furcoat);
  //ArithmeticIC platypus_minus_otter;
  //substract(platypus_minus_otter,"n01873310 platypus, duckbill, duckbilled platypus, duck-billed platypus, Ornithorhynchus anatinus", "n02444819 otter", scheme, imageClasses);
  //ariths.push_back(platypus_minus_otter);
  //ArithmeticIC platypus_minus_albatros;
  //substract(platypus_minus_albatros,"n01873310 platypus, duckbill, duckbilled platypus, duck-billed platypus, Ornithorhynchus anatinus", "n02058221 albatross, mollymawk", scheme, imageClasses);
  //ariths.push_back(platypus_minus_albatros);
  //ArithmeticIC platypus_minus_drake;
  //substract(platypus_minus_drake,"n01873310 platypus, duckbill, duckbilled platypus, duck-billed platypus, Ornithorhynchus anatinus", "n01847000 drake", scheme, imageClasses);
  //ariths.push_back(platypus_minus_drake);
  //ArithmeticIC marmot_plus_albatros;
  //add(marmot_plus_albatros,"n02361337 marmot", "n02058221 albatross, mollymawk", scheme, imageClasses);
  //ariths.push_back(marmot_plus_albatros);
  //ArithmeticIC otter_plus_albatros;
  //add(otter_plus_albatros,"n02444819 otter", "n02058221 albatross, mollymawk", scheme, imageClasses);
  //ariths.push_back(otter_plus_albatros);
  //ArithmeticIC beaver_plus_albatros;
  //add(beaver_plus_albatros,"n02363005 beaver", "n02058221 albatross, mollymawk", scheme, imageClasses);
  //ariths.push_back(beaver_plus_albatros);
  //ArithmeticIC marmot_plus_drake;
  //add(marmot_plus_drake,"n02361337 marmot", "n01847000 drake", scheme, imageClasses);
  //ariths.push_back(marmot_plus_drake);
  //ArithmeticIC otter_plus_drake;
  //add(otter_plus_drake,"n02444819 otter", "n01847000 drake", scheme, imageClasses);
  //ariths.push_back(otter_plus_drake);
  //ArithmeticIC beaver_plus_drake;
  //add(beaver_plus_drake,"n02363005 beaver", "n01847000 drake", scheme, imageClasses);
  //ariths.push_back(beaver_plus_drake);
  //ArithmeticIC gecko_plus_armor;
  //add(gecko_plus_armor,"n01675722 banded gecko", "n03000247 chain mail, ring mail, mail, chain armor, chain armour, ring armor, ring armour", scheme, imageClasses);
  //ariths.push_back(gecko_plus_armor);
  //ArithmeticIC iguana_plus_armor;
  //add(iguana_plus_armor,"n01677366 common iguana, iguana, Iguana iguana", "n03000247 chain mail, ring mail, mail, chain armor, chain armour, ring armor, ring armour", scheme, imageClasses);
  //ariths.push_back(iguana_plus_armor);
  //ArithmeticIC agama_plus_armor;
  //add(agama_plus_armor,"n01687978 agama", "n03000247 chain mail, ring mail, mail, chain armor, chain armour, ring armor, ring armour", scheme, imageClasses);
  //ariths.push_back(agama_plus_armor);
  //ArithmeticIC turtle_minus_armor;
  //substract(turtle_minus_armor,"n01667778 terrapin", "n03000247 chain mail, ring mail, mail, chain armor, chain armour, ring armor, ring armour", scheme, imageClasses);
  //ariths.push_back(turtle_minus_armor);
  //ArithmeticIC turtle2_minus_armor;
  //substract(turtle2_minus_armor,"n01669191 box turtle, box tortoise", "n03000247 chain mail, ring mail, mail, chain armor, chain armour, ring armor, ring armour", scheme, imageClasses);
  //ariths.push_back(turtle2_minus_armor);
  //ArithmeticIC gecko_plus_cuirass;
  //add(gecko_plus_cuirass,"n01675722 banded gecko", "n03146219 cuirass", scheme, imageClasses);
  //ariths.push_back(gecko_plus_cuirass);
  //ArithmeticIC iguana_plus_cuirass;
  //add(iguana_plus_cuirass,"n01677366 common iguana, iguana, Iguana iguana", "n03146219 cuirass", scheme, imageClasses);
  //ariths.push_back(iguana_plus_cuirass);
  //ArithmeticIC agama_plus_cuirass;
  //add(agama_plus_cuirass,"n01687978 agama", "n03146219 cuirass", scheme, imageClasses);
  //ariths.push_back(agama_plus_cuirass);
  //ArithmeticIC turtle_minus_cuirass;
  //substract(turtle_minus_cuirass,"n01667778 terrapin", "n03146219 cuirass", scheme, imageClasses);
  //ariths.push_back(turtle_minus_cuirass);
  //ArithmeticIC turtle2_minus_cuirass;
  //substract(turtle2_minus_cuirass,"n01669191 box turtle, box tortoise", "n03146219 cuirass", scheme, imageClasses);
  //ariths.push_back(turtle2_minus_cuirass);


  //ArithmeticIC black_white;
  //substract(black_white,"n02510455 giant panda, panda, panda bear, coon bear, Ailuropoda melanoleuca", "n02132136 brown bear, bruin, Ursus arctos", scheme, imageClasses);
  //ariths.push_back(black_white);

  //ArithmeticIC skunk_minu_black_white;
  //substract(skunk_minu_black_white,"n02445715 skunk, polecat, wood pussy", black_white, scheme, imageClasses);
  //ariths.push_back(skunk_minu_black_white);

  //ArithmeticIC angora_black_white;
  //substract(angora_black_white,"n02328150 Angora, Angora rabbit", black_white, scheme, imageClasses);
  //ariths.push_back(angora_black_white);

  //ArithmeticIC soccer_black_white;
  //substract(soccer_black_white,"n04254680 soccer ball", black_white, scheme, imageClasses);
  //ariths.push_back(soccer_black_white);

  //ArithmeticIC indri_black_white;
  //substract(indri_black_white,"n02500267 indri, indris, Indri indri, Indri brevicaudatus", black_white, scheme, imageClasses);
  //ariths.push_back(indri_black_white);

  
  time(&t_end);
  printf("MAIN::Compute arithmetics took %f\n",difftime (t_end,t_init));


  //The whole ArithmeticImageClass code is not elegant or efficient. Needs work (get rid of sets, for starters)
  if(distanceType==1)printf("MAIN::Using euclidean distance\n");
  if(distanceType==2)printf("MAIN::Using cosine distance\n");
  for(int i = 0 ; i<ariths.size();i++){
    printf("iessim %u has name %s\n",i,ariths[i].first.getName().c_str());
    pair<ImageClass,double> closestIC = findClosestArithmeticClass(imageClasses, ariths[i], scheme, distanceType);
  }
  time(&t_end);
  printf("MAIN::Compute closest classes to arithmetic classes took %f\n",difftime (t_end,t_init));


  
}  

