# script by A.Villano
# commented
# 
# 2019 Apr 15: save out vars from photoneutron condensed root trees to hdf5 

import numpy as np
#from scipy.stats import gaussian_kde
import sys
import uproot
import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)
import h5py
warnings.resetwarnings()

outfilename = sys.argv[1] #first argument is output file name
zip = sys.argv[2] #first argument is output file name
flist=[] #empty list for files

for n,i in enumerate(sys.argv):
  if n>2:
    flist.append(i)

print(flist)

#pntree = TChain('edeptree_zip{}'.format(zip),'edeptree_zip{}'.format(zip))

#for i in flist:
#  pntree.Add(i)


pnfile = uproot.open(flist[0])
zipstring = 'edeptree_zip{}'.format(zip)
print(zipstring)
ziptree = pnfile[zipstring]
a = ziptree.arrays(['nhit'])
nev = np.shape(a[b'nhit'])[0]
print(nev)


#print(pntree.GetEntries())
#pntree.GetEntry(0)
#print(pntree.NRhit)
#print(pntree.NRedep[0])
#arr = pntree.NRedep
#NRedep = np.array(arr)
#print(np.shape(NRedep))
#sys.exit(0)

#start making data structures
#nev = pntree.GetEntries()
maxnr = 13
maxer = 30
maxcap = 1
maxprim =1
output = {};
output['NRhit'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['ERhit'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['ncap'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['nprim'] = np.ones((nev,),dtype=int)*-1 #start out with negative ones
output['totalevents'] = np.zeros((1,)) 
output['NRedep'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRYield'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRx'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRy'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRz'] = np.zeros((nev,maxnr)) #start out with zeros 
output['NRt'] = np.zeros((nev,maxnr)) #start out with zeros 
output['cNRcapProg'] = np.zeros((nev,maxnr),dtype=bool) #start out with False 
output['ERedep'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERYield'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERx'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERy'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERz'] = np.zeros((nev,maxer)) #start out with zeros 
output['ERt'] = np.zeros((nev,maxer)) #start out with zeros 
output['cERcapProg'] = np.zeros((nev,maxer),dtype=bool) #start out with False 
output['prim_PType'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_KE'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_X'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Y'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Z'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Xmom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Ymom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['prim_Zmom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_KE'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_X'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Y'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Z'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Time'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Xmom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Ymom'] = np.zeros((nev,maxprim)) #start out with zeros 
output['cap_Zmom'] = np.zeros((nev,maxprim)) #start out with zeros 

print('There are: {}\n'.format(nev))
#nevt = 100000 

#pntree.GetEntry(0)
#output['totalevents'] = pntree.totalevents
allarrays = ziptree.arrays(['totalevents','NRhit','ERhit','ncap','nprim','NRedep','NRYield','NRx','NRy','NRz','NRt','NRcapProg','ERedep','ERYield','ERx','ERy','ERz','ERt','ERcapProg','prim_PType','prim_KE','prim_X','prim_Y','prim_Z','prim_Xmom','prim_Ymom','prim_Zmom','cap_KE','cap_X','cap_Y','cap_Z','cap_Time','cap_Xmom','cap_Ymom','cap_Zmom'])
#for jevt in range(pntree.GetEntries()):
#for entry,vec in enumerate(allarrays[b'NRedep']):
for entry in range(nev):
    print('processing event {} of {}\r'.format(entry,nev))
    #pntree.GetEntry(entry))

    output['totalevents'][0] = allarrays[b'totalevents'][entry]
    output['NRhit'][entry] = allarrays[b'NRhit'][entry]
    output['ERhit'][entry] = allarrays[b'ERhit'][entry]
    output['ncap'][entry] = allarrays[b'ncap'][entry] 
    output['nprim'][entry] = allarrays[b'nprim'][entry]

    vl = np.min([allarrays[b'NRhit'][entry],maxnr])
    output['NRedep'][entry][0:vl] = allarrays[b'NRedep'][entry][0:vl] 
    output['NRYield'][entry][0:vl] = allarrays[b'NRYield'][entry][0:vl] 
    output['NRx'][entry][0:vl] = allarrays[b'NRx'][entry][0:vl] 
    output['NRy'][entry][0:vl] = allarrays[b'NRy'][entry][0:vl] 
    output['NRz'][entry][0:vl] = allarrays[b'NRz'][entry][0:vl] 
    output['NRt'][entry][0:vl] = allarrays[b'NRt'][entry][0:vl] 
    output['cNRcapProg'][entry][0:vl] = allarrays[b'NRcapProg'][entry][0:vl] 
    #for i in np.arange(np.min([pntree.NRhit,maxnr])):
    #  #if pntree.ncap>0:
    #  #  print('{} iteration for event {}; NR energy: {}'.format(i,entry,pntree.NRedep[i]))
    #  output['NRedep'][entry][i] = pntree.NRedep[i]
    #  output['NRYield'][entry][i] = pntree.NRYield[i]
    #  output['NRx'][entry][i] = pntree.NRx[i]
    #  output['NRy'][entry][i] = pntree.NRy[i]
    #  output['NRz'][entry][i] = pntree.NRz[i]
    #  output['NRt'][entry][i] = pntree.NRt[i]
    #  output['cNRcapProg'][entry][i] = np.bool(pntree.NRcapProg[i])

    #if pntree.ncap>0:
    #  print(output['NRedep'][entry])
    vl = np.min([allarrays[b'ERhit'][entry],maxer])
    output['ERedep'][entry][0:vl] = allarrays[b'ERedep'][entry][0:vl] 
    output['ERYield'][entry][0:vl] = allarrays[b'ERYield'][entry][0:vl] 
    output['ERx'][entry][0:vl] = allarrays[b'ERx'][entry][0:vl] 
    output['ERy'][entry][0:vl] = allarrays[b'ERy'][entry][0:vl] 
    output['ERz'][entry][0:vl] = allarrays[b'ERz'][entry][0:vl] 
    output['ERt'][entry][0:vl] = allarrays[b'ERt'][entry][0:vl] 
    output['cERcapProg'][entry][0:vl] = allarrays[b'ERcapProg'][entry][0:vl] 
    
    #for i in np.arange(np.min([pntree.ERhit,maxer])):
    #  output['ERedep'][entry][i] = pntree.ERedep[i]
    #  output['ERYield'][entry][i] = pntree.ERYield[i]
    #  output['ERx'][entry][i] = pntree.ERx[i]
    #  output['ERy'][entry][i] = pntree.ERy[i]
    #  output['ERz'][entry][i] = pntree.ERz[i]
    #  output['ERt'][entry][i] = pntree.ERt[i]
    #  output['cERcapProg'][entry][i] = np.bool(pntree.ERcapProg[i])
    
    vl = np.min([allarrays[b'nprim'][entry],maxprim])
    output['prim_PType'][entry][0:vl] = allarrays[b'prim_PType'][entry][0:vl] 
    output['prim_KE'][entry][0:vl] = allarrays[b'prim_KE'][entry][0:vl] 
    output['prim_X'][entry][0:vl] = allarrays[b'prim_X'][entry][0:vl] 
    output['prim_Y'][entry][0:vl] = allarrays[b'prim_Y'][entry][0:vl] 
    output['prim_Z'][entry][0:vl] = allarrays[b'prim_Z'][entry][0:vl] 
    output['prim_Xmom'][entry][0:vl] = allarrays[b'prim_Xmom'][entry][0:vl] 
    output['prim_Ymom'][entry][0:vl] = allarrays[b'prim_Ymom'][entry][0:vl] 
    output['prim_Zmom'][entry][0:vl] = allarrays[b'prim_Zmom'][entry][0:vl] 

    #for i in np.arange(np.min([pntree.nprim,maxprim])):
    #  output['prim_PType'][entry][i] = pntree.prim_PType[i]
    #  output['prim_KE'][entry][i] = pntree.prim_KE[i]
    #  output['prim_X'][entry][i] = pntree.prim_X[i]
    #  output['prim_Y'][entry][i] = pntree.prim_Y[i]
    #  output['prim_Z'][entry][i] = pntree.prim_Z[i]
    #  output['prim_Xmom'][entry][i] = pntree.prim_Xmom[i]
    #  output['prim_Ymom'][entry][i] = pntree.prim_Ymom[i]
    #  output['prim_Zmom'][entry][i] = pntree.prim_Zmom[i]

    vl = np.min([allarrays[b'ncap'][entry],maxcap])
    output['cap_KE'][entry][0:vl] = allarrays[b'cap_KE'][entry][0:vl] 
    output['cap_X'][entry][0:vl] = allarrays[b'cap_X'][entry][0:vl] 
    output['cap_Y'][entry][0:vl] = allarrays[b'cap_Y'][entry][0:vl] 
    output['cap_Z'][entry][0:vl] = allarrays[b'cap_Z'][entry][0:vl] 
    output['cap_Time'][entry][0:vl] = allarrays[b'cap_Time'][entry][0:vl] 
    output['cap_Xmom'][entry][0:vl] = allarrays[b'cap_Xmom'][entry][0:vl] 
    output['cap_Ymom'][entry][0:vl] = allarrays[b'cap_Ymom'][entry][0:vl] 
    output['cap_Zmom'][entry][0:vl] = allarrays[b'cap_Zmom'][entry][0:vl] 

    #for i in np.arange(np.min([pntree.ncap,maxcap])):
    #  output['cap_KE'][entry][i] = pntree.cap_KE[i]
    #  output['cap_X'][entry][i] = pntree.cap_X[i]
    #  output['cap_Y'][entry][i] = pntree.cap_Y[i]
    #  output['cap_Z'][entry][i] = pntree.cap_Z[i]
    #  output['cap_Time'][entry][i] = pntree.cap_Time[i]
    #  output['cap_Xmom'][entry][i] = pntree.cap_Xmom[i]
    #  output['cap_Ymom'][entry][i] = pntree.cap_Ymom[i]
    #  output['cap_Zmom'][entry][i] = pntree.cap_Zmom[i]

    ## debugging...
    #if (jevt % 10000) == 0:
    #  print 'processing event ',jevt,' in mcDecays'
    #    print '... its volume is ',volname.tostring()
# end loop over decay tree

#open and write file
of = h5py.File(outfilename, 'w')

for k,v in output.items():
  print(k)
  print(np.shape(v))
  dset_hits = of.create_dataset("simdata/{}".format(k), np.shape(v), dtype=np.dtype('float64').type, compression="gzip", compression_opts=9)
  dset_hits[...] = v

of.close()
exit(1)
#outfile = file('/data/chocula/villaa/PhotoN_SuperSim/possys/flux_data_v2.pkl', 'w')
outfile = file(outfilename, 'w')
pickle.dump(output, outfile)
outfile.close()

