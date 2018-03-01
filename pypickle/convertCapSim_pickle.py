# script by A.Villano
# commented
# 
# 2018 Mar 1: to get the true capture spectrum I've used files like:
#             /data/chocula/villaa/cascadeSimData/normsi_fast.root.
#             this code puts them into a format useable for the PN simulation;
#             specifically, I want to replace capture events with these.

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

cascadetree = TChain('cascade','cascade')
escapetree = TChain('escapeInfo','escapeInfo')

for i in flist:
  cascadetree.Add(i)
  escapetree.Add(i)


#print(cascadetree.GetEntries())
#cascadetree.GetEntry(0)
#print(cascadetree.NRhit)
#print(cascadetree.NRedep[0])
#arr = cascadetree.NRedep
#NRedep = np.array(arr)
#print(np.shape(NRedep))
#sys.exit(0)

#start making data structures
nev = cascadetree.GetEntries()
nev = np.min([10000,nev]) #cut down on events
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
output['NRx'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRy'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRz'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRt'] = np.zeros((nev,maxnr)) #start out with zeros 
output['cNRcapProg'] = np.zeros((nev,maxnr),dtype=bool) #start out with False 
output['cEscape'] = np.zeros((nev,),dtype=bool) #start out with False 
output['ERedep'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERYield'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERx'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERy'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERz'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERt'] = np.zeros((nev,maxer)) #start out with zeros 
output['cERcapProg'] = np.zeros((nev,maxer),dtype=bool) #start out with False 

print 'There are: {}\n'.format(nev) 

cascadetree.GetEntry(0)
output['totalevents'] = nev 
#for jevt in range(cascadetree.GetEntries()):
for entry in range(nev):
    print 'processing event {} of {}\r'.format(entry,nev),
    cascadetree.GetEntry(entry)

    output['NRhit'][entry] = cascadetree.n
    output['ERhit'][entry] = 0 
    output['cEscape'][entry] = np.bool(escapetree.didInteract) 
    for i in np.arange(np.min([cascadetree.n,maxnr])):
      #if pntree.ncap>0:
      #  print('{} iteration for event {}; NR energy: {}'.format(i,entry,pntree.NRedep[i]))
      output['NRedep'][entry][i] = cascadetree.E[i]
      output['NRYield'][entry][i] = cascadetree.Ei[i]/cascadetree.E[i]
      output['NRx'][entry][i] = 0 
      output['NRy'][entry][i] = 0 
      output['NRz'][entry][i] = 0 
      output['NRt'][entry][i] = 0 
      output['cNRcapProg'][entry][i] = True 

    #if pntree.ncap>0:
    #  print(output['NRedep'][entry])

    #currently there are not ERs in this file
    for i in np.arange(np.min([0,maxer])):
      output['ERedep'][entry][i] = 0 
      output['ERYield'][entry][i] = 0 
      output['ERx'][entry][i] = 0 
      output['ERy'][entry][i] = 0 
      output['ERz'][entry][i] = 0 
      output['ERt'][entry][i] = 0 
      output['cERcapProg'][entry][i] = True 
    
    ## debugging...
    #if (jevt % 10000) == 0:
    #  print 'processing event ',jevt,' in mcDecays'
    #    print '... its volume is ',volname.tostring()
# end loop over decay tree


#outfile = file('/data/chocula/villaa/PhotoN_SuperSim/possys/flux_data_v2.pkl', 'w')
outfile = file(outfilename, 'w')
pickle.dump(output, outfile)
outfile.close()

