# script by A.Villano
# commented
# 
# 2018 Feb 24: save out vars from photoneutron condensed root trees 

from ROOT import TFile, TTree, TChain
#import ROOT.std
from array import array
import numpy as np
import cPickle as pickle
from scipy.stats import gaussian_kde
import sys

outfilename = sys.argv[1] #first argument is output file name
flist=[] #empty list for files

for n,i in enumerate(sys.argv):
  if n>1:
    flist.append(i)

print(flist)

pntree = TChain('edeptree_zip14','edeptree_zip14')

for i in flist:
  pntree.Add(i)


#print(pntree.GetEntries())
#pntree.GetEntry(0)
#print(pntree.NRhit)
#print(pntree.NRedep[0])
#arr = pntree.NRedep
#NRedep = np.array(arr)
#print(np.shape(NRedep))
#sys.exit(0)

#start making data structures
nev = pntree.GetEntries()
maxnr = 13
maxer = 30
maxcap = 1
maxprim =1
output = {};
output['NRhit'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['ERhit'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['ncap'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['nprim'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['totalevents'] = np.int(-1) 
output['NRedep'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRYield'] = np.zeros((nev,maxnr)) #start out with zeros 
output['ERedep'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERYield'] = np.zeros((nev,maxer)) #start out with zeros 
output['prim_PType'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_KE'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_X'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Y'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Z'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Xmom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Ymom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Zmom'] = np.zeros((nev,maxprim)) #start out with zeros 
#output['cap_KE'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_X'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Y'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Z'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Time'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Xmom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Ymom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Zmom'] = np.zeros((nev,maxprim)) #start out with zeros 

print 'There are: {}\n'.format(nev) 
#nevt = 100000 

pntree.GetEntry(0)
output['totalevents'] = pntree.totalevents
#for jevt in range(pntree.GetEntries()):
for entry in range(nev):
    print 'processing event {} of {}\r'.format(entry,nev),
    pntree.GetEntry(entry)

    output['NRhit'][entry] = pntree.NRhit
    output['ERhit'][entry] = pntree.ERhit
    output['ncap'][entry] = pntree.ncap
    output['nprim'][entry] = pntree.ncap
    for i in np.arange(np.min([pntree.NRhit,maxnr])):
      output['NRedep'][entry][i] = pntree.NRedep[i]
      output['NRYield'][entry][i] = pntree.NRYield[i]

    for i in np.arange(np.min([pntree.ERhit,maxer])):
      output['ERedep'][entry][i] = pntree.ERedep[i]
      output['ERYield'][entry][i] = pntree.ERYield[i]
    
    for i in np.arange(np.min([pntree.nprim,maxprim])):
      output['prim_PType'][entry][i] = pntree.prim_PType[i]
      output['prim_KE'][entry][i] = pntree.prim_KE[i]
      output['prim_X'][entry][i] = pntree.prim_X[i]
      output['prim_Y'][entry][i] = pntree.prim_Y[i]
      output['prim_Z'][entry][i] = pntree.prim_Z[i]
      output['prim_Xmom'][entry][i] = pntree.prim_Xmom[i]
      output['prim_Ymom'][entry][i] = pntree.prim_Ymom[i]
      output['prim_Zmom'][entry][i] = pntree.prim_Zmom[i]

    for i in np.arange(np.min([pntree.ncap,maxcap])):
      #output['cap_KE'][entry][i] = pntree.cap_KE[i]
      output['cap_X'][entry][i] = pntree.cap_X[i]
      output['cap_Y'][entry][i] = pntree.cap_Y[i]
      output['cap_Z'][entry][i] = pntree.cap_Z[i]
      output['cap_Time'][entry][i] = pntree.cap_Time[i]
      output['cap_Xmom'][entry][i] = pntree.cap_Xmom[i]
      output['cap_Ymom'][entry][i] = pntree.cap_Ymom[i]
      output['cap_Zmom'][entry][i] = pntree.cap_Zmom[i]

    ## debugging...
    #if (jevt % 10000) == 0:
    #  print 'processing event ',jevt,' in mcDecays'
    #    print '... its volume is ',volname.tostring()
# end loop over decay tree


#outfile = file('/data/chocula/villaa/PhotoN_SuperSim/possys/flux_data_v2.pkl', 'w')
outfile = file(outfilename, 'w')
pickle.dump(output, outfile)
outfile.close()

