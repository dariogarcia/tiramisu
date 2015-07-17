import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import linkage, dendrogram, leaves_list
import numpy as np
import csv
from sklearn import manifold

inf = open('labels.txt', 'r')
labels = list()

#Read labels
for line in inf:
  labels.append(line)

#Build empty list of lists
sim = list()
for i in range(1000):
  onesim = [0]*1000
  sim.append(onesim)

#Fill with the elements in their right order
inf2 = open('/media/dario/823D-2FFA/data_tiramisu/arith_cosine_classlayer_distances.dat','r')
for line in inf2:
  words = line.split('---')
  word1 = words[0]+'\n'
  word2 = words[1]+'\n'
  word12sim = words[2]
  sim[labels.index(word1)][labels.index(word2)] = word12sim
  sim[labels.index(word2)][labels.index(word1)] = word12sim
  

#for c1 in range(1000):
#  for c2 in range(1000):
#    if sim[c1][c2] != sim[c2][c1]:
#      print "diff " + str(labels[c1]) + " " + str(labels[c2])


dogs = []
dogs.append("n02110341 dalmatian, coach dog, carriage dog\n")
dogs.append("n02113978 Mexican hairless\n")
dogs.append("n02111277 Newfoundland, Newfoundland dog\n")
dogs.append("n02110806 basenji\n")
dogs.append("n02111129 Leonberg\n")
dogs.append("n02110958 pug, pug-dog\n")
dogs.append("n02111500 Great Pyrenees\n")
dogs.append("n02087394 Rhodesian ridgeback\n")
dogs.append("n02100583 vizsla, Hungarian pointer\n")
dogs.append("n02100236 German short-haired pointer\n")
dogs.append("n02100735 English setter\n")
dogs.append("n02100877 Irish setter, red setter\n")
dogs.append("n02101006 Gordon setter\n")
dogs.append("n02101556 clumber, clumber spaniel\n")
dogs.append("n02101388 Brittany spaniel\n")
dogs.append("n02102318 cocker spaniel, English cocker spaniel, cocker\n")
dogs.append("n02102480 Sussex spaniel\n")
dogs.append("n02102973 Irish water spaniel\n")
dogs.append("n02102040 English springer, English springer spaniel\n")
dogs.append("n02102177 Welsh springer spaniel\n")
dogs.append("n02099601 golden retriever\n")
dogs.append("n02099849 Chesapeake Bay retriever\n")
dogs.append("n02099429 curly-coated retriever\n")
dogs.append("n02099267 flat-coated retriever\n")
dogs.append("n02099712 Labrador retriever\n")
dogs.append("n02096294 Australian terrier\n")
dogs.append("n02098105 soft-coated wheaten terrier\n")
dogs.append("n02096437 Dandie Dinmont, Dandie Dinmont terrier\n")
dogs.append("n02096051 Airedale, Airedale terrier\n")
dogs.append("n02098413 Lhasa, Lhasa apso\n")
dogs.append("n02094433 Yorkshire terrier\n")
dogs.append("n02098286 West Highland white terrier\n")
dogs.append("n02094114 Norfolk terrier\n")
dogs.append("n02096177 cairn, cairn terrier\n")
dogs.append("n02093859 Kerry blue terrier\n")
dogs.append("n02097298 Scotch terrier, Scottish terrier, Scottie\n")
dogs.append("n02096585 Boston bull, Boston terrier\n")
dogs.append("n02093647 Bedlington terrier\n")
dogs.append("n02093991 Irish terrier\n")
dogs.append("n02097658 silky terrier, Sydney silky\n")
dogs.append("n02094258 Norwich terrier\n")
dogs.append("n02097474 Tibetan terrier, chrysanthemum dog\n")
dogs.append("n02093754 Border terrier\n")
dogs.append("n02095314 wire-haired fox terrier\n")
dogs.append("n02095570 Lakeland terrier\n")
dogs.append("n02095889 Sealyham terrier, Sealyham\n")
dogs.append("n02097047 miniature schnauzer\n")
dogs.append("n02097130 giant schnauzer\n")
dogs.append("n02097209 standard schnauzer\n")
dogs.append("n02093256 Staffordshire bullterrier, Staffordshire bull terrier\n")
dogs.append("n02093428 American Staffordshire terrier, Staffordshire terrier, American pit bull terrier, pit bull terrier\n")
dogs.append("n02092002 Scottish deerhound, deerhound\n")
dogs.append("n02092339 Weimaraner\n")
dogs.append("n02088466 bloodhound, sleuthhound\n")
dogs.append("n02091635 otterhound, otter hound\n")
dogs.append("n02091467 Norwegian elkhound, elkhound\n")
dogs.append("n02091831 Saluki, gazelle hound\n")
dogs.append("n02088094 Afghan hound, Afghan\n")
dogs.append("n02088364 beagle\n")
dogs.append("n02088238 basset, basset hound\n")
dogs.append("n02088632 bluetick\n")
dogs.append("n02090379 redbone\n")
dogs.append("n02091244 Ibizan hound, Ibizan Podenco\n")
dogs.append("n02090721 Irish wolfhound\n")
dogs.append("n02090622 borzoi, Russian wolfhound\n")
dogs.append("n02089078 black-and-tan coonhound\n")
dogs.append("n02089867 Walker hound, Walker foxhound\n")
dogs.append("n02089973 English foxhound\n")
dogs.append("n02091032 Italian greyhound\n")
dogs.append("n02091134 whippet\n")
dogs.append("n02113023 Pembroke, Pembroke Welsh corgi\n")
dogs.append("n02113186 Cardigan, Cardigan Welsh corgi\n")
dogs.append("n02113624 toy poodle\n")
dogs.append("n02113712 miniature poodle\n")
dogs.append("n02113799 standard poodle\n")
dogs.append("n02112706 Brabancon griffon\n")
dogs.append("n02109047 Great Dane\n")
dogs.append("n02108089 boxer\n")
dogs.append("n02108422 bull mastiff\n")
dogs.append("n02109961 Eskimo dog, husky\n")
dogs.append("n02109525 Saint Bernard, St Bernard\n")
dogs.append("n02105641 Old English sheepdog, bobtail\n")
dogs.append("n02106382 Bouvier des Flandres, Bouviers des Flandres\n")
dogs.append("n02106550 Rottweiler\n")
dogs.append("n02105505 komondor\n")
dogs.append("n02106030 collie\n")
dogs.append("n02106166 Border collie\n")
dogs.append("n02105855 Shetland sheepdog, Shetland sheep dog, Shetland\n")
dogs.append("n02105412 kelpie\n")
dogs.append("n02105251 briard\n")
dogs.append("n02106662 German shepherd, German shepherd dog, German police dog, alsatian\n")
dogs.append("n02105162 malinois\n")
dogs.append("n02105056 groenendael\n")
dogs.append("n02104365 schipperke\n")
dogs.append("n02104029 kuvasz\n")
dogs.append("n02107142 Doberman, Doberman pinscher\n")
dogs.append("n02107312 miniature pinscher\n")
dogs.append("n02110627 affenpinscher, monkey pinscher, monkey dog\n")
dogs.append("n02110185 Siberian husky\n")
dogs.append("n02110063 malamute, malemute, Alaskan malamute\n")
dogs.append("n02108000 EntleBucher\n")
dogs.append("n02107683 Bernese mountain dog\n")
dogs.append("n02107574 Greater Swiss Mountain dog\n")
dogs.append("n02107908 Appenzeller\n")
dogs.append("n02108551 Tibetan mastiff\n")
dogs.append("n02108915 French bulldog\n")
dogs.append("n02112018 Pomeranian\n")
dogs.append("n02112350 keeshond\n")
dogs.append("n02112137 chow, chow chow\n")
dogs.append("n02111889 Samoyed, Samoyede\n")
dogs.append("n02086079 Pekinese, Pekingese, Peke\n")
dogs.append("n02085936 Maltese dog, Maltese terrier, Maltese\n")
dogs.append("n02087046 toy terrier\n")
dogs.append("n02085782 Japanese spaniel\n")
dogs.append("n02086240 Shih-Tzu\n")
dogs.append("n02085620 Chihuahua\n")
dogs.append("n02086910 papillon\n")
dogs.append("n02086646 Blenheim spaniel\n")

#strings = []
#strings.append("n02787622 banjo\n")
#strings.append("n02992211 cello, violoncello\n")
#strings.append("n04536866 violin, fiddle\n")
#strings.append("n03495258 harp\n")
#strings.append("n02676566 acoustic guitar\n")
#strings.append("n03272010 electric guitar\n")
#
#wind = []
#wind.append("n03840681 ocarina, sweet potato\n")
#wind.append("n03110669 cornet, horn, trumpet, trump\n")
#wind.append("n03394916 French horn, horn\n")
#wind.append("n04487394 trombone\n")
#wind.append("n03494278 harmonica, mouth organ, harp, mouth harp\n")
#wind.append("n03884397 panpipe, pandean pipe, syrinx\n")
#wind.append("n03372029 flute, transverse flute\n")
#wind.append("n02804610 bassoon\n")
#wind.append("n03838899 oboe, hautboy, hautbois\n")
#wind.append("n04141076 sax, saxophone\n")
#wind.append("n04579432 whistle\n")

wheeled = []
wheeled.append("n03791053 motor scooter, scooter\n")
wheeled.append("n04482393 tricycle, trike, velocipede\n")
wheeled.append("n04509417 unicycle, monocycle\n")
wheeled.append("n02835271 bicycle-built-for-two, tandem bicycle, tandem\n")
wheeled.append("n03792782 mountain bike, all-terrain bike, off-roader\n")
wheeled.append("n03393912 freight car\n")
wheeled.append("n03895866 passenger car, coach, carriage\n")
wheeled.append("n04204347 shopping cart\n")
wheeled.append("n02797295 barrow, garden cart, lawn cart, wheelbarrow\n")
wheeled.append("n03384352 forklift\n")
wheeled.append("n04065272 recreational vehicle, RV, R.V.\n")
wheeled.append("n04335435 streetcar, tram, tramcar, trolley, trolley car\n")
wheeled.append("n04465501 tractor\n")
wheeled.append("n04389033 tank, army tank, armored combat vehicle, armoured combat vehicle\n")
wheeled.append("n03272562 electric locomotive\n")
wheeled.append("n04310018 steam locomotive\n")
wheeled.append("n02704792 amphibian, amphibious vehicle\n")
wheeled.append("n03444034 go-kart\n")
wheeled.append("n03445924 golfcart, golf cart\n")
wheeled.append("n04252225 snowplow, snowplough\n")
wheeled.append("n02701002 ambulance\n")
wheeled.append("n02814533 beach wagon, station wagon, wagon, estate car, beach waggon, station waggon, waggon\n")
wheeled.append("n02930766 cab, hack, taxi, taxicab\n")
wheeled.append("n03100240 convertible\n")
wheeled.append("n03594945 jeep, landrover\n")
wheeled.append("n03670208 limousine, limo\n")
wheeled.append("n03770679 minivan\n")
wheeled.append("n03777568 Model T\n")
wheeled.append("n04285008 sports car, sport car\n")
wheeled.append("n04037443 racer, race car, racing car\n")
wheeled.append("n03785016 moped\n")
wheeled.append("n03417042 garbage truck, dustcart\n")
wheeled.append("n03345487 fire engine, fire truck\n")
wheeled.append("n03930630 pickup, pickup truck\n")
wheeled.append("n04461696 tow truck, tow car, wrecker\n")
wheeled.append("n04467665 trailer truck, tractor trailer, trucking rig, rig, articulated lorry, semi\n")
wheeled.append("n03796401 moving van\n")
wheeled.append("n03977966 police van, police wagon, paddy wagon, patrol wagon, wagon, black Maria\n")
wheeled.append("n03478589 half track\n")
wheeled.append("n04252077 snowmobile\n")
wheeled.append("n03776460 mobile home, manufactured home\n")
wheeled.append("n03538406 horse cart, horse-cart\n")
wheeled.append("n03599486 jinrikisha, ricksha, rickshaw\n")
wheeled.append("n03868242 oxcart\n")
wheeled.append("n04487081 trolleybus, trolley coach, trackless trolley\n")
wheeled.append("n03769881 minibus\n")
wheeled.append("n04146614 school bus\n")

l = []
for i in range (1000):
  l.append(map(float,sim[i]))

adist = np.array(l)
amax = np.amax(l)
adist /= amax

colors = []
for lab in labels:
  if lab in dogs:
    colors.append("k")
  elif lab in wheeled:
    colors.append("0.5")
#  elif lab in strings:
#    colors.append(2)
#  elif lab in wind:
#    colors.append(3)
  else:
    colors.append("0.9")


mds = manifold.MDS(n_components=2, dissimilarity="precomputed", random_state=6)
results = mds.fit(adist)

coords = results.embedding_

plt.figure(figsize=(60,60))
plt.subplots_adjust(bottom = 0.1)
plt.scatter(
    coords[:, 0], coords[:, 1], marker = 'o', s=2000, c=colors
    )
plt.gray()
##for label, x, y in zip(labels, coords[:, 0], coords[:, 1]):
##  plt.annotate(
##    label,
##    xy = (x, y), xytext = (-20, 20),
##    textcoords = 'offset points', ha = 'right', va = 'bottom',
##    bbox = dict(boxstyle = 'round,pad=0.5', fc = 'yellow', alpha = 0.5),
##    arrowprops = dict(arrowstyle = '->', connectionstyle = 'arc3,rad=0'))

plt.show()
plt.savefig('/media/dario/823D-2FFA/data_tiramisu/arith_cosine_classlayer_graph.pdf', format='pdf')

