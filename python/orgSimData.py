#this library provides functions to organize various simulation data beginning with sim v4 ID 0x0004
import numpy as np
import pickle
import math

def getFirstData_Simv4(loadFiles=False,suppressBig=False):
 
    #go through a bunch of data and load it
    prefixdir = '/data/chocula/villaa/PhotoN_SuperSim/ZipSum'
    data = {}
    data['z14'] = {}
    data['z14']['ybe'] = {}
    data['z14']['ybe']['typelabel'] = 'Y/Be'
    data['z14']['ybe']['CDMSliteV'] = 70.0
    data['z14']['ybe']['bigfile'] = 'ybe_z14_180223_0x0004_1-3B.pkl'
    data['z14']['ybe']['smallfile'] = 'ybe_z14_180223_0x0004_100M.pkl'
    data['z14']['sbbe'] = {}
    data['z14']['sbbe']['typelabel'] = 'Sb/Be'
    data['z14']['sbbe']['CDMSliteV'] = 70.0
    data['z14']['sbbe']['bigfile'] = 'sbbe_z14_180302_0x0004_1-3B.pkl'
    data['z14']['sbbe']['smallfile'] = 'sbbe_z14_180302_0x0004_100M.pkl'
    data['z4'] = {}
    data['z4']['ybe'] = {}
    data['z4']['ybe']['typelabel'] = 'Y/Be'
    data['z4']['ybe']['CDMSliteV'] = 25.0
    data['z4']['ybe']['bigfile'] = 'ybe_z4_180223_0x0004_1-3B.pkl'
    data['z4']['ybe']['smallfile'] = 'ybe_z4_180223_0x0004_100M.pkl'
    data['z4']['sbbe'] = {}
    data['z4']['sbbe']['typelabel'] = 'Sb/Be'
    data['z4']['sbbe']['CDMSliteV'] = 25.0
    data['z4']['sbbe']['bigfile'] = 'sbbe_z4_180302_0x0004_1-3B.pkl'
    data['z4']['sbbe']['smallfile'] = 'sbbe_z4_180302_0x0004_100M.pkl'
    data['z5'] = {}
    data['z5']['ybe'] = {}
    data['z5']['ybe']['typelabel'] = 'Y/Be'
    data['z5']['ybe']['CDMSliteV'] = 0.0
    data['z5']['ybe']['bigfile'] = 'ybe_z5_180305_0x0004_1-3B.pkl'
    data['z5']['ybe']['smallfile'] = 'ybe_z5_180305_0x0004_100M.pkl'
    data['z5']['sbbe'] = {}
    data['z5']['sbbe']['typelabel'] = 'Sb/Be'
    data['z5']['sbbe']['CDMSliteV'] = 0.0
    data['z5']['sbbe']['bigfile'] = 'sbbe_z5_180305_0x0004_1-3B.pkl'
    data['z5']['sbbe']['smallfile'] = 'sbbe_z5_180305_0x0004_100M.pkl'
    data['z14']['ybe']['capfile-Ge'] = 'ybe_z14_180307_cap_slow.pkl' 
    cfile = prefixdir + '/' + data['z14']['ybe']['capfile-Ge']

    if loadFiles:
      with open(cfile,'rb') as readFile:
          data['z14']['ybe']['capdata-Ge']=pickle.load(readFile,encoding='latin1')
    
    
      for det in data:
          print('reading file for detector {}'.format(det))
      
          for set in data[det]:
            file = prefixdir + '/' + data[det][set]['smallfile']
            file2 = prefixdir + '/' + data[det][set]['bigfile']
            print(file)
            print(file2)
            with open(file,'rb') as readFile:
                  data[det][set]['smalldata']=pickle.load(readFile,encoding='latin1')
            if not suppressBig:
              with open(file2,'rb') as readFile:
                    data[det][set]['bigdata']=pickle.load(readFile,encoding='latin1')
                  
      #add the flux data
      for set in data['z14']:
          data['z14'][set]['fluxfile'] = '/data/chocula/villaa/PhotoN_SuperSim/ZipSum/flux_data_'+set+'_z14_0x000a_v1.pkl'
          file = data['z14'][set]['fluxfile']
          with open(file,'rb') as readFile:
                  data['z14'][set]['fluxdata']=pickle.load(readFile,encoding='latin1')
     
    #set some names
    data['z14']['detname'] = 'IT5Z2'
    data['z4']['detname'] = 'IT2Z1'
    data['z5']['detname'] = 'IT2Z2'
    
    #add trigger thresholds
    data['z14']['ybe']['CDMSlite'] = {}
    data['z14']['ybe']['CDMSlite']['thresh']=350.
    data['z14']['ybe']['CDMSlite']['sigthr']=20. # sigma on threshold
    data['z14']['ybe']['CDMSlite']['thresh_t']=1800.
    data['z14']['ybe']['CDMSlite']['sigthr_t']=100. # sigma on threshold
    data['z14']['sbbe']['CDMSlite'] = {}
    data['z14']['sbbe']['CDMSlite']['thresh']=350.
    data['z14']['sbbe']['CDMSlite']['sigthr']=20. # sigma on threshold
    data['z14']['sbbe']['CDMSlite']['thresh_t']=1800.
    data['z14']['sbbe']['CDMSlite']['sigthr_t']=100. # sigma on threshold
    data['z4']['ybe']['CDMSlite'] = {}
    data['z4']['ybe']['CDMSlite']['thresh']=890.
    data['z4']['ybe']['CDMSlite']['sigthr']=40. # sigma on threshold
    data['z4']['ybe']['CDMSlite']['thresh_t']=2200.
    data['z4']['ybe']['CDMSlite']['sigthr_t']=100. # sigma on threshold
    data['z4']['sbbe']['CDMSlite'] = {}
    data['z4']['sbbe']['CDMSlite']['thresh']=890.
    data['z4']['sbbe']['CDMSlite']['sigthr']=40. # sigma on threshold
    data['z4']['sbbe']['CDMSlite']['thresh_t']=2200.
    data['z4']['sbbe']['CDMSlite']['sigthr_t']=100. # sigma on threshold

    return data


def condenseFirstData_Simv4(file=None,suppressBig=False):

    #load the first data set
    data = getFirstData_Simv4(True,suppressBig)


    datalabel = 'bigdata'
    datalabel_r = 'bigdata_r'

    if suppressBig:
      datalabel = 'smalldata'
      datalabel_r = 'smalldata_r'

    #iterate through data and do replacements
    data_condense = {}
    for det in data:

      #set up output structure
      if det not in data_condense:
        data_condense[det] = {}

      for set in data[det]:
       
        if set=='detname' : continue #I stored some info at that level

	#set up output structure
        if set not in data_condense[det]:
          data_condense[det][set] = {}

	#replace the captures
        seedStr = det + set #a reproducible string to get seed for shuffling of events
        data[det][set][datalabel_r] = replaceCaptures(data[det][set][datalabel],seedStr) #correct file is default
        data_condense[det][set] = {}
        data_condense[det][set]['NRedep'] = data[det][set][datalabel_r]['NRedep'] 
        data_condense[det][set]['cEscape'] = data[det][set][datalabel_r]['cEscape'] 
        data_condense[det][set]['ncap'] = data[det][set][datalabel_r]['ncap']
        data_condense[det][set]['NRhit'] = data[det][set][datalabel_r]['NRhit']
        data_condense[det][set]['ERhit'] = data[det][set][datalabel_r]['ERhit']

    if file is not None: 
      outfile = open(file, 'wb')
      pickle.dump(data_condense, outfile)
      outfile.close()

    return data_condense


def replaceCaptures(data, seedString='1', file='/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_z14_180307_cap_slow.pkl'): #data is standard .pkl simulation detector sub-structure

    #open the capture file
    with open(file,'rb') as readFile:
      capdata=pickle.load(readFile,encoding='latin1')

    print(capdata.keys())

    #sum the NR hits from the Geant4 simulation
    edepNR = np.sum(data['NRedep'],1)
    
    #get the full structure with all hits as well
    NRedep = data['NRedep']

    #take the number of total captures (with or without ER)
    oneper = np.ones(np.shape(data['ncap']))
    Ncap = np.sum(oneper[data['ncap']>0])
    
    print('Capture structure shapes and first capture BEFORE replacement================')
    print(Ncap)
    print(np.shape(edepNR))
    print(np.shape(NRedep))
  
  
    #go ahead and pick out this many events from the capture file
    #but need to reshuffle them because the capture data has each cascade simulated in order
    idx_orig = np.arange(np.shape(data['NRedep'])[0])
    idx = np.arange(np.shape(capdata['NRedep'])[0]) 
    
    #careful with this, use a consistent seed to get consistent dataset
    #here it gives the same number of indicies independent of the seed, however later 
    #when we cut out the ones that didn't have all gammas escape, the dataset will
    #be different sized unless you use a consistent random state
    rstate = np.random.RandomState()
    repint = int.from_bytes(seedString.encode(), 'little') % (2**32-1) #reproducible based on the string input seedString
    print(repint) #reproducible based on the string input seedString
    rstate.seed(repint) #reproducible based on the string input seedString
    idx_rand = rstate.choice(idx,np.int(Ncap),replace=False)
    
    #get indicies of capture in original vector 
    idx_cap = idx_orig[data['ncap']>0]
    print(NRedep[idx_cap[0]])
    
    
    #get summed data structure and radomize
    edepNR_cap = np.sum(capdata['NRedep'],1)
    
    #replace cap events in original structure
    edepNR[idx_cap] = edepNR_cap[idx_rand]
    NRedep[idx_cap] = capdata['NRedep'][idx_rand]
    
    print('')
    print('Capture structure shapes and first capture AFTER replacement================')
    print(Ncap)
    print(np.shape(edepNR))
    print(np.shape(NRedep))
    print(NRedep[idx_cap[0]])

    #now have to expand the cut cEscape to cover the whole dataset
    cEscape = np.ones(np.shape(edepNR),dtype=bool) #default is all escaped
    cEscape[idx_cap] = capdata['cEscape'][idx_rand]
    
    print(np.shape(cEscape))
    oneper = np.ones(np.shape(cEscape))
    NnotEscape = np.sum(oneper[~cEscape])
    print('Retained {0:d} escaping captures, should be same number for multiple runs of this notebook' \
          .format(np.int(Ncap-NnotEscape)))

    #replace all the relevant variables in data
    data['NRedep'] = NRedep
    data['cEscape'] = cEscape

    return data 
