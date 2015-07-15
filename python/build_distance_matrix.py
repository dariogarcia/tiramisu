from nltk.corpus import wordnet as wn
from nltk.corpus import genesis
from scipy.stats import spearmanr
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt



##Distances from file
inf = open('labels.txt', 'r')
labels = list()
labelsNLTK = list()
synsets = list()

#Read labels and transform into NLTK compatible
for line in inf:
  labels.append(line)
  synsets.append((line.split()[0])[1:]+'-n')
  labelsNLTK.append((line.split()[0])[1:]+'-n')

#Build empty list of lists
sim = list()
for i in range(1000):
  mysim = [0]*1000
  sim.append(mysim)

#Fill with the elements in their right order
inf2 = open('/home/dariog/gits/data-tiramisu/arith_cosine_class_distances.dat','r')
for line in inf2:
  words = line.split('---')
  word1 = words[0]+'\n'
  word2 = words[1]+'\n'
  word12sim = words[2]
  sim[labels.index(word1)][labels.index(word2)] = word12sim
print 'done building distance matrix according to results'


##Distances from wordnet

distances_path = list()
distances_lch = list()
distances_wup = list()
distances_res = list()
distances_jcn = list()
distances_lin = list()
distances_res_bnc = list()
distances_jcn_bnc = list()
distances_lin_bnc = list()

#Build distance matrix structure
for i in range(1000):
  sim1 = [0]*1000
  sim2 = [0]*1000
  sim3 = [0]*1000
  sim4 = [0]*1000
  sim5 = [0]*1000
  sim6 = [0]*1000
  sim7 = [0]*1000
  sim8 = [0]*1000
  sim9 = [0]*1000
  distances_path.append(sim1)
  distances_lch.append(sim2)
  distances_wup.append(sim3)
  distances_res.append(sim4)
  distances_jcn.append(sim5)
  distances_lin.append(sim6)
  distances_res_bnc.append(sim7)
  distances_jcn_bnc.append(sim8)
  distances_lin_bnc.append(sim9)

#Import IC calculation
from nltk.corpus import wordnet_ic
brown_ic = wordnet_ic.ic('ic-brown-resnik-add1.dat')
bnc_ic = wordnet_ic.ic('ic-bnc-resnik-add1.dat')

#For each pair of synsets, compute distance
for s1 in synsets:
  syn1 = wn.of2ss(s1)
  for s2 in synsets:
    syn2 = wn.of2ss(s2)
    distances_path[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.path_similarity(syn1,syn2)
    distances_lch[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.lch_similarity(syn1,syn2)
    distances_wup[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.wup_similarity(syn1,syn2)
    distances_res[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.res_similarity(syn1,syn2,brown_ic)
    distances_jcn[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.jcn_similarity(syn1,syn2,brown_ic)
    distances_lin[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.lin_similarity(syn1,syn2,brown_ic)
    distances_res_bnc[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.res_similarity(syn1,syn2,bnc_ic)
    distances_jcn_bnc[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.jcn_similarity(syn1,syn2,bnc_ic)
    distances_lin_bnc[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1 - wn.lin_similarity(syn1,syn2,bnc_ic)
    #distances_path[labelsNLTK.index(s1)][labelsNLTK.index(s2)] =1/(labelsNLTK.index(s2)+1) 
    #distances_lch[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)
    #distances_wup[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)  
    #distances_res[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)  
    #distances_jcn[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)  
    #distances_lin[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)  
    #distances_res_bnc[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)  
    #distances_jcn_bnc[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)  
    #distances_lin_bnc[labelsNLTK.index(s1)][labelsNLTK.index(s2)] = 1/(labelsNLTK.index(s2)+1)  

print 'done computing wordnet distances'
     
#plt.figure()
fig, axes = plt.subplots(nrows=3, ncols=3)
ax0, ax1, ax2, ax3, ax4, ax5, ax6, ax7, ax8 = axes.flat
bins = [-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.2,-0.2,-0.1,0,0.1,0.2,0.2,0.4,0.5,0.6,0.7,0.8,0.9,1]


print "Values path"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_path[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho path:' + labels(i) + str(rho_sci)
#hist, bin_edges = np.histogram(rhos,bins=[-1,-0.5,0, 0.2, 0.4,0.5, 0.6, 0.7,1])
#print hist
n, bins, patches = ax0.hist(rhos, bins, normed=1, histtype='stepfilled')
ax0.set_title('Path')
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
print "\n"
print "\n"



print "Values lch"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_lch[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho lch:' + labels(i) + str(rho_sci)
n, bins, patches = ax1.hist(rhos, bins, normed=1, histtype='stepfilled')
ax1.set_title('LCh')
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
print "\n"
print "\n"


print "Values wup"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_wup[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho wup:' + labels(i) + str(rho_sci)
n, bins, patches = ax2.hist(rhos, bins, normed=1, histtype='stepfilled')
ax2.set_title('WuP')
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
print "\n"
print "\n"


print "Values res brown"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_res[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho res brown:' + labels(i) + str(rho_sci)
n, bins, patches = ax3.hist(rhos, bins, normed=1, histtype='stepfilled')
ax3.set_title('Res brown')
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
print "\n"
print "\n"


print "Values jcn brown"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_jcn[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho jcn brown:' + labels(i) + str(rho_sci)
n, bins, patches = ax4.hist(rhos, bins, normed=1, histtype='stepfilled')
ax4.set_title('JCn brown')
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
print "\n"
print "\n"


print "Values lin brown"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_lin[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho lin brown:' + labels(i) + str(rho_sci)
n, bins, patches = ax5.hist(rhos, bins, normed=1, histtype='stepfilled')
ax5.set_title('Lin brown')
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
print "\n"
print "\n"


print "Values res bnc"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_res_bnc[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho res bnc:' + labels(i) + str(rho_sci)
n, bins, patches = ax6.hist(rhos, bins, normed=1, histtype='stepfilled')
ax6.set_title('Res bnc')
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
print "\n"
print "\n"


print "Values jcn bnc"
print "\n"
pvalues = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_jcn_bnc[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho jcn bnc:' + labels(i) + str(rho_sci)
n, bins, patches = ax7.hist(rhos, bins, normed=1, histtype='stepfilled')
ax7.set_title('JCn bnc')
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
print "\n"
print "\n"



print "Values lin bnc"
print "\n"
pvalues = list()
rhos = list()
rhos = list()
total_rho_sci = 0
for i in range(1000):
  rho_sci, pval = spearmanr(sim[i],distances_lin_bnc[i])
  pvalues.append(pval)
  total_rho_sci+=rho_sci
  rhos.append(rho_sci)
  if rho_sci < 0.2:
    print 'Small Rho lin bnc:' + labels(i) + str(rho_sci)
n, bins, patches = ax8.hist(rhos, bins, normed=1, histtype='stepfilled')
ax8.set_title('Lin bnc')
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
print "\n"
print "\n"



plt.tight_layout()
plt.show()
plt.savefig('../../data-tiramisu/arith_cosine_class_histogram.pdf', format='pdf')
#plt.tight_layout()
#plt.show()
#plt.savefig('arith_layer_histogram.pdf', format='pdf')

#total_rho_rpy = 0
  #tho_rpy = rpy.r.cor(sim[i],distances_path[i],method="spearman")
  #total_rho_rpy+=rho_rpy
#print 'total rho rpy'
#print total_rho_rpy



