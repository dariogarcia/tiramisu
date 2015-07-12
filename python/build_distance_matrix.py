from nltk.corpus import wordnet as wn
from nltk.corpus import genesis
from scipy.stats import spearmanr
import numpy as np
#import rpy2


inf = open('labels.txt', 'r')
labels = list()
labelsNLTK = list()
synsets = list()
distances_path = list()
distances_lch = list()
distances_wup = list()
distances_res = list()
distances_jcn = list()
distances_lin = list()

#Build distance matrix structure
for i in range(1000):
  onesim = [0]*1000
  distances_path.append(onesim)
  twosim = [0]*1000
  distances_lch.append(twosim)
  threesim = [0]*1000
  distances_wup.append(threesim)
  foursim = [0]*1000
  distances_res.append(foursim)
  fivesim = [0]*1000
  distances_jcn.append(fivesim)
  sixsim = [0]*1000
  distances_lin.append(sixsim)

#Read labels and transform into NLTK compatible
for line in inf:
  labels.append(line)
  synsets.append((line.split()[0])[1:]+'-n')
  labelsNLTK.append((line.split()[0])[1:]+'-n')

#Import IC calculation
#from nltk.corpus import wordnet_ic
#brown_ic = wordnet_ic.ic('ic-brown-resnik-add1.dat')
#bnc_ic = wordnet_ic.ic('ic-bnc-resnik-add1.dat')

#For each pair of synsets, compute distance
for s1 in synsets:
  syn1 = wn.of2ss(s1)
  for s2 in synsets:
    syn2 = wn.of2ss(s2)
    distances_path[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.path_similarity(syn1,syn2)
    #distances_lch[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.lch_similarity(syn1,syn2)
    #distances_wup[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.wup_similarity(syn1,syn2)
    #distances_res[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.res_similarity(syn1,syn2,brown_ic)
    #distances_jcn[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.jcn_similarity(syn1,syn2,brown_ic)
    #distances_lin[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.lin_similarity(syn1,syn2,brown_ic)
    #distances_res[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.res_similarity(syn1,syn2,bnc_ic)
    #distances_jcn[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.jcn_similarity(syn1,syn2,bnc_ic)
    #distances_lin[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.lin_similarity(syn1,syn2,bnc_ic)
    #print str(labels[labelsNLTK.index(s1)]) + ' ' + str(labels[labelsNLTK.index(s2)]) + ' ' + str(wn.path_similarity(syn1,syn2))

print 'done computing wordnet distances'
     
#Build empty list of lists
sim = list()
for i in range(1000):
  mysim = [0]*1000
  sim.append(mysim)

#Fill with the elements in their right order
inf2 = open('/home/dario/dist_459.out','r')
for line in inf2:
  words = line.split('---')
  word1 = words[0]+'\n'
  word2 = words[1]+'\n'
  word12sim = words[2]
  sim[labels.index(word1)][labels.index(word2)] = word12sim
print 'done building distance matrix according to results'

pvalues = list()
total_rho_sci = 0
#total_rho_rpy = 0
for i in range(1000):
  #print 'values of ' + str(labels[i]) 
  rho_sci, pval = spearmanr(sim[i],distances_path[i])
  pvalues.append(pval)
  total_rho_sci+=np.absolute(rho_sci)
  #tho_rpy = rpy.r.cor(sim[i],distances_path[i],method="spearman")
  #total_rho_rpy+=np.absolute(rho_rpy)

print 'min pvalues'
print np.amin(pvalues)
print 'max pvalues'
print np.amax(pvalues)
print 'mean pvalues'
print np.mean(pvalues)
print 'stdDev pvalues'
print np.std(pvalues)
print 'total rho sci'
print total_rho_sci
#print 'total rho rpy'
#print total_rho_rpy
