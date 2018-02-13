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
nevt = decaytree.GetEntries()
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
for jevt in range(nevt):
    #print 'processing event %d of %d\r' % (jevt,nevt)
    print 'processing event {} of {}\r'.format(jevt,nevt),
    decaytree.GetEntry(jevt)
    decays['EventNum'][jevt] = EventNum[0]
    decays['Edep'][jevt] = Edep[0]
    decays['x'][jevt] = x[0]
    decays['y'][jevt] = y[0]
    decays['z'][jevt] = z[0]
    decays['px'][jevt] = px[0]
    decays['py'][jevt] = py[0]
    decays['pz'][jevt] = pz[0]
    decays['time'][jevt] = time[0]
    decays['ptype'][jevt] = ptype[0]
    decays['inout'][jevt] = inout[0]
    # (DB) cannot add in strings in the same manner
    decays['volname'] = np.append(decays['volname'], volname.tostring())
    ## debugging...
    #if (jevt % 100000) == 0:
    #    print 'processing event ',jevt,' in mcDecays'
    #    print '... its volume is ',volname.tostring()
# end loop over decay tree


outfile = file('/data/chocula/villaa/PhotoN_SuperSim/possys/flux_data_v2.pkl', 'w')
pickle.dump(decays, outfile)
outfile.close()
