# script by A.Villano
# commented
# 
# 2018 Feb 13: save out vars from supersim mcFluxCounter tree 

from ROOT import TFile, TTree
#import ROOT.std
from array import array
import numpy as np
import cPickle as pickle
import sys

infilename = sys.argv[1] #first argument is file name
outfilename = sys.argv[2] #second argument is output file name
infile = TFile(infilename, 'READ')

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
decaytree = infile.Get('G4SimDir/mcFluxCounter')
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
decaytree.Draw('X1','InOut==3 || InOut==2','goff')
nevt = decaytree.GetSelectedRows()
print 'There are: {}\n'.format(nevt) 
#nevt = 100000 

# (DB) dictionaries need keys and values: making the keys here...
decays = dict()
decays['EventNum'] = np.zeros(nevt)
decays['Edep'] = np.zeros(nevt)
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
c=0;
for jevt in range(decaytree.GetEntries()):
    #print 'processing event %d of %d\r' % (jevt,nevt)
    print 'processing event {} of {}\r'.format(jevt,decaytree.GetEntries()),
    decaytree.GetEntry(jevt)

    #only interested in events that have an exiting boundary crossing
    if((inout[0]==2) | (inout[0]==3)):
      decays['EventNum'][c] = EventNum[0]
      decays['Edep'][c] = Edep[0]
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
      #decays['volname'] = np.append(decays['volname'], volname.tostring())
      c+=1
    
    ## debugging...
    #if (jevt % 10000) == 0:
    #  print 'processing event ',jevt,' in mcDecays'
    #    print '... its volume is ',volname.tostring()
# end loop over decay tree


#outfile = file('/data/chocula/villaa/PhotoN_SuperSim/possys/flux_data_v2.pkl', 'w')
outfile = file(outfilename, 'w')
pickle.dump(decays, outfile)
outfile.close()
