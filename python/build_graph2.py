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
inf2 = open('/home/dariog/gits/data-tiramisu/arith_cosine_classlayer_distances.dat','r')
for line in inf2:
  words = line.split('---')
  word1 = words[0]+'\n'
  word2 = words[1]+'\n'
  word12sim = words[2]
  sim[labels.index(word1)][labels.index(word2)] = word12sim
  #sim[labels.index(word2)][labels.index(word1)] = word12sim
  

for c1 in range(1000):
  for c2 in range(1000):
    if sim[c1][c2] != sim[c2][c1]:
      print "diff " + str(labels[c1]) + " " + str(labels[c2])


l = []
for i in range (1000):
  l.append(map(float,sim[i]))

adist = np.array(l)
amax = np.amax(l)
adist /= amax




mds = manifold.MDS(n_components=2, dissimilarity="precomputed", random_state=6)
results = mds.fit(adist)

coords = results.embedding_

plt.figure(figsize=(120,120))
plt.subplots_adjust(bottom = 0.1)
plt.scatter(
    coords[:, 0], coords[:, 1], marker = 'o'
    )
for label, x, y in zip(labels, coords[:, 0], coords[:, 1]):
    plt.annotate(
        label,
        xy = (x, y), xytext = (-20, 20),
        textcoords = 'offset points', ha = 'right', va = 'bottom',
        bbox = dict(boxstyle = 'round,pad=0.5', fc = 'yellow', alpha = 0.5),
        arrowprops = dict(arrowstyle = '->', connectionstyle = 'arc3,rad=0'))

plt.show()
plt.savefig('/home/dariog/gits/data-tiramisu/arith_cosine_classlayer_graph.pdf', format='pdf')

