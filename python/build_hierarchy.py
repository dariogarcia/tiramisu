import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import linkage, dendrogram, leaves_list
import numpy as np

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
inf2 = open('/tmp/dist_208.out','r')
for line in inf2:
  words = line.split('---')
  word1 = words[0]+'\n'
  word2 = words[1]+'\n'
  word12sim = words[2]
  sim[labels.index(word1)][labels.index(word2)] = word12sim
  
#print sim

#Compute hierarchical clustering
z = linkage(sim,method='average')
plt.figure(figsize=(12,120))
d =dendrogram(z,p=20,
        truncate_mode='none',
        show_contracted=True,
        leaf_font_size=5,
        orientation='left',
        #get_leaves=True,
        #show_leaf_counts=True,
        labels=labels)

#Print dendrogram
#print d['ivl']
leaves = leaves_list(z)
#print leaves

#clusters = d['ivl']
#clusters_len = len(clusters)
#cluster_count = 0
#i=0
#while i<999:
#  if not "(" in str(clusters[cluster_count]):
#    print "Cluster " + str(cluster_count)
#    #print labels[int(clusters[cluster_count])]
#    print clusters[cluster_count]
#    print "End Cluster "+ str(cluster_count)
#    cluster_count+=1
#    i+=1
#  else:
#    cluster_size = str(clusters[cluster_count]).replace('(','')
#    cluster_size2 = cluster_size.replace(')','')
#    print "Cluster "+ str(cluster_count) + " with id " + cluster_size2
#    for j in range(0, int(cluster_size2)):
#      print labels[int(leaves[i])]
#      i+=1
#    cluster_count+=1
#    print "End Cluster "+ str(cluster_count)

#plt.show()
plt.subplots_adjust(left=0.4)
plt.savefig('hierarchy_208.pdf', format='pdf')