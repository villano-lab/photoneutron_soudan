# script by A.Villano
# commented
# 
# 2018 Mar 02: save out vars from supersim mcFluxCounter tree for zip neutron flux

from ROOT import TFile, TTree, TChain
#import ROOT.std
from array import array
import numpy as np
import cPickle as pickle
from scipy.stats import gaussian_kde
import sys
from vecCalc import *

outfilename = sys.argv[1] #first argument is output file name
#infilename = sys.argv[1] #first argument is file name
flist=[] #empty list for files

#print(dotProd([1,2,3],[1,2,3]))
#print(np.linalg.norm([1,2,3]))
#a = [1,2]
#print(np.linalg.norm(a))
#a = a/np.linalg.norm(a)
#print(np.linalg.norm(a))
#print(cosCalc([-1,-1,-1],[5,5,5]))
#sys.exit(0)
for n,i in enumerate(sys.argv):
  if n>1:
    flist.append(i)

print(flist)

decaytree = TChain('G4SimDir/mcFluxCounter','G4SimDir/mcFluxCounter')
infotree = TChain('G4SettingsInfoDir/runtime','G4SettingsInfoDir/runtime')

for i in flist:
  decaytree.Add(i)
  infotree.Add(i)

#decaytree.Draw('X1','InOut==3 || InOut==2','goff')
#nevt = decaytree.GetSelectedRows()
#print 'There are: {}\n'.format(nevt) 

#infile = TFile(infilename, 'READ')

EventNum = array('d', [0.])
Edep = array('d', [0.])
KE = array('d', [0.])
x = array('d', [0.])
y = array('d', [0.])
z = array('d', [0.])
px = array('d', [0.])
py = array('d', [0.])
pz = array('d', [0.])
time = array('d', [0.])
ptype = array('d', [0.])
inout = array('d', [0.])
volname = array('c',list('Zip'))

# extract times and particle types from the decays tree in order
# to group hits by type
#decaytree = infile.Get('G4SimDir/mcFluxCounter')
decaytree.SetBranchAddress('EventNum', EventNum)
decaytree.SetBranchAddress('Edep', Edep)
decaytree.SetBranchAddress('KE', KE)
decaytree.SetBranchAddress('X3', x)
decaytree.SetBranchAddress('Y3', y)
decaytree.SetBranchAddress('Z3', z)
decaytree.SetBranchAddress('Xmom3', px)
decaytree.SetBranchAddress('Ymom3', py)
decaytree.SetBranchAddress('Zmom3', pz)
decaytree.SetBranchAddress('Time3', time)
decaytree.SetBranchAddress('PType', ptype)
decaytree.SetBranchAddress('InOut', inout)
decaytree.SetBranchAddress('VolName', volname)
#nevt = decaytree.GetEntries()
decaytree.SetEstimate(decaytree.GetEntries())
decaytree.Draw('Entry$*(InOut>0)','InOut!=0','goff')
#decaytree.Draw('Entry$*(InOut>0)','','goff')
#decaytree.Draw('Entry$','(InOut==3 || InOut==2)&&Z3<0.9','goff')
nevt = decaytree.GetSelectedRows()
ent=decaytree.GetV1()
idx = range(nevt)
print 'There are: {}\n'.format(nevt) 
#nevt = 100000 

# (DB) dictionaries need keys and values: making the keys here...
decays = dict()
decays['totalevents'] = np.int(-1) 

#get the total number of primaries
decays['totalevents']=0
for c in range(infotree.GetEntries()):
  infotree.GetEntry(c)
  decays['totalevents']+=infotree.Events

decays['EventNum'] = np.zeros(nevt)
decays['Edep'] = np.zeros(nevt)
decays['ke'] = np.zeros(nevt)
decays['x'] = np.zeros(nevt)
decays['y'] = np.zeros(nevt)
decays['z'] = np.zeros(nevt)
decays['px'] = np.zeros(nevt)
decays['py'] = np.zeros(nevt)
decays['pz'] = np.zeros(nevt)
decays['time'] = np.zeros(nevt)
decays['ptype'] = np.zeros(nevt)
decays['inout'] = np.zeros(nevt)
decays['volname'] = np.array([], dtype=str)
# (DB) ...now filling in the values 
#c=0;
#for jevt in range(decaytree.GetEntries()):
for c in range(nevt):
    #print 'processing event %d of %d\r' % (jevt,nevt)
    #print 'processing event {} of {}\r'.format(jevt,decaytree.GetEntries()),
    print 'processing event {} of {}\r'.format(c,nevt),
    #decaytree.GetEntry(jevt)
    entry = int(ent[c])
    decaytree.GetEntry(entry)

    #only interested in events that have an exiting boundary crossing
    if(inout[0]!=0):
      decays['EventNum'][c] = EventNum[0]
      decays['Edep'][c] = Edep[0]
      decays['ke'][c] = KE[0]
      decays['x'][c] = x[0]
      decays['y'][c] = y[0]
      decays['z'][c] = z[0]
      decays['px'][c] = px[0]
      decays['py'][c] = py[0]
      decays['pz'][c] = pz[0]
      decays['time'][c] = time[0]
      decays['ptype'][c] = ptype[0]
      decays['inout'][c] = inout[0]
      # (DB) cannot add in strings in the same manner
      decays['volname'] = np.append(decays['volname'], volname.tostring())
    
    ## debugging...
    #if (jevt % 10000) == 0:
    #  print 'processing event ',jevt,' in mcDecays'
    #    print '... its volume is ',volname.tostring()
# end loop over decay tree

#put some extra information in, because it's computationally intensive
# Calculate the point density
x = decays['x']
y = decays['y']
z = decays['z']

#decrease them if over 100k points--to keep KDE to a reasonable time
randcut = np.ones(np.shape(x),dtype=bool)
if(len(x)>100000):
  print 'limiting number of points to 100k'
  fac = 100000.0/float(len(x))
  randvec = np.random.rand(*np.shape(x))
  randcut[randvec>fac] = False 

xlim = x[randcut]
ylim = y[randcut]
zlim = z[randcut]

xyz = np.vstack([x,y,z])
xyzlim = np.vstack([xlim,ylim,zlim])
#print(np.shape(xyzlim))
#print(xyzlim[np.arange(3)][np.arange(3)])
kde_3d = gaussian_kde(xyzlim)
color = kde_3d(xyz)

# Sort the points by density, so that the densest points are plotted last
idx = color.argsort()
#x, y, z, color = x[idx], y[idx], z[idx], color[idx]
#decays['color'] = color

#re-order all the vectors
decays['color'] = color[idx]
decays['EventNum'], decays['Edep'],  decays['x'],  decays['y'],  decays['z'],  decays['px'],  decays['py'],  decays['pz'],  decays['time'],  decays['ptype'],  decays['inout'], decays['volname'] = decays['EventNum'][idx], decays['Edep'][idx],  decays['x'][idx],  decays['y'][idx],  decays['z'][idx],  decays['px'][idx],  decays['py'][idx],  decays['pz'][idx],  decays['time'][idx],  decays['ptype'][idx],  decays['inout'][idx], decays['volname'][idx] 

#why don't we save the kde
decays['flux_kde'] = kde_3d

#outfile = file('/data/chocula/villaa/PhotoN_SuperSim/possys/flux_data_v2.pkl', 'w')
outfile = file(outfilename, 'w')
pickle.dump(decays, outfile)
outfile.close()

