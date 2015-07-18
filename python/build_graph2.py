import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import linkage, dendrogram, leaves_list
import numpy as np
import csv
from sklearn import manifold
from nltk.corpus import wordnet as wn
from nltk.corpus import genesis

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

#Read labels and transform into NLTK compatible
inf = open('labels.txt', 'r')
labels = list()
synsets = list()
labelsNLTK = list()
for line in inf:
  labels.append(line)
  labelsNLTK.append((line.split()[0])[1:]+'-n')

#Obtain synsets
for s1 in labelsNLTK:
  synsets.append(wn.of2ss(s1))

d_s = wn.synset('dog.n.01').closure(lambda s:s.hyponyms())
dogs = []
for d in d_s:
  dogs.append(d)

w_s = wn.synset('wheeled_vehicle.n.01').closure(lambda s:s.hyponyms())
wheeled = []
for w in w_s:
  wheeled.append(w)
wheeled.append(wn.synset('school_bus.n.01'))
wheeled.append(wn.synset('minibus.n.01'))
wheeled.append(wn.synset('trolleybus.n.01'))

a_s = wn.synset('animal.n.01').closure(lambda s:s.hyponyms())
animals = []
for a in a_s:
  animals.append(a)
#print "Num animals "+ str(len(animals))

c_s = wn.synset('chordate.n.01').closure(lambda s:s.hyponyms())
chordates = []
for c in c_s:
  chordates.append(c)
#print "Num chordates "+ str(len(chordates))

i_s = wn.synset('invertebrate.n.01').closure(lambda s:s.hyponyms())
invertebrates = []
for i in i_s:
  invertebrates.append(i)
#print "Num  invertebrates "+ str(len(invertebrates))

ma_s = wn.synset('aquatic_vertebrate.n.01').closure(lambda s:s.hyponyms())
marine_animals = []
for ma in ma_s:
  marine_animals.append(ma)
#print "Num aquatic vertebrates "+ str(len(marine_animals))

v_s = wn.synset('vertebrate.n.01').closure(lambda s:s.hyponyms())
vertebrates = []
for v in v_s:
  vertebrates.append(v)
#print "Num vertebrates "+ str(len(vertebrates))

b_s = wn.synset('bird.n.01').closure(lambda s:s.hyponyms())
birds = []
for b in b_s:
  birds.append(b)
#print "Num birds "+ str(len(birds))

r_s = wn.synset('reptile.n.01').closure(lambda s:s.hyponyms())
reptiles = []
for r in r_s:
  reptiles.append(r)
#print "Num reptiles "+ str(len(reptiles))

vertebrates_nobird_nomarine_noreptile = list(vertebrates)
for b in birds:
  vertebrates_nobird_nomarine_noreptile.remove(b)
for m in marine_animals:
  vertebrates_nobird_nomarine_noreptile.remove(m)
for r in reptiles:
  vertebrates_nobird_nomarine_noreptile.remove(r)
#print "Num vertebreates no-bird/marine/reptile "+ str(len(vertebrates_nobird_nomarine_noreptile))

#animal 3998 
#  chordate 3087
#    vertebrate non-bird non-aquatic non-reptile 1278
#    bird 871
#    reptile 296
#    aquatic vertebrate 632
#    ? 10
#  invertebrate 766
#  animals_others 145


l = []
for i in range (1000):
  l.append(map(float,sim[i]))

adist = np.array(l)
amax = np.amax(l)
adist /= amax

colors = []
for lab in labels:
  labelNLTK = (lab.split()[0])[1:]+'-n'
  syn = wn.of2ss(labelNLTK)
#  if syn in chordates:
#    colors.append("r")
#  elif syn in invertebrates:
#    colors.append("k")
#  else:
#    colors.append("w")

#  if syn in vertebrates_nobird_nomarine_noreptile:
#    colors.append("r")
#  elif syn in reptiles:
#    colors.append("g")
#  elif syn in birds:
#    colors.append("y")
#  elif syn in marine_animals:
#    colors.append("b")
#  elif syn in invertebrates:
#    colors.append("k")
#  else:
#    colors.append("w")

  if syn in animals:
    colors.append("y")
  else:
    colors.append("0.9")

#  if syn in dogs:
#    colors.append("k")
#  elif syn in wheeled:
#    colors.append("0.5")
#  else:
#    colors.append("0.9")


mds = manifold.MDS(n_components=2, dissimilarity="precomputed", random_state=6)
results = mds.fit(adist)

coords = results.embedding_

plt.figure(figsize=(60,60))
plt.subplots_adjust(bottom = 0.1)
plt.scatter(
    coords[:, 0], coords[:, 1], marker = 'o', s=3000, c=colors
    )
#plt.gray()
for label, x, y in zip(labels, coords[:, 0], coords[:, 1]):
  labelNLTK = (label.split()[0])[1:]+'-n'
  syn = wn.of2ss(labelNLTK)
  if syn not in animals:
    plt.annotate(
      label,
      xy = (x, y), xytext = (-10, 10),
      textcoords = 'offset points', ha = 'right', va = 'bottom',
      bbox = dict(boxstyle = 'round,pad=0.5', fc = 'yellow', alpha = 0.5),
      arrowprops = dict(arrowstyle = '->', connectionstyle = 'arc3,rad=0'))

plt.show()
plt.savefig('/media/dario/823D-2FFA/data_tiramisu/arith_cosine_classlayer_graph_tmp.pdf', format='pdf')

